

(provide 
 string-hash make-hashtable make-eqv-hashtable hash-info hashtable-set! 
 hashtable-size test-hash)

;; a base size for the hash
(define hash-base-size 4)
(define hash-scale-factor 2)
(define max-load-factor 0.7)

;; a very crappy hash function
(define (string-hash str)
  (define hash 0)

  (for-each (lambda (x) 
              (set! hash
                    (+ x (* 32 hash))))
            (map char->integer (string->list str)))
  hash)


;; construct a hash table
(define (make-hashtable hash-proc equiv-proc)
  (vector hash-proc equiv-proc (make-vector hash-base-size) 0))

(define (make-eqv-hashtable)
  (make-hashtable string-hash eqv?))

;; constants defining where things are in the hashtable structure
(define hash-proc 0)
(define equiv-proc 1)
(define hash-vector 2)
(define hash-entries 3)


(define (load-factor table) 
  (/ (fixnum->floatnum (hashtable-size table)) 
     (vector-length (vector-ref table hash-vector))))

;; display some information about a hash table
(define (hash-info table)
  (newline)
  (display "hash-proc:")
  (display (vector-ref table hash-proc))
  (newline)
  (display "equiv-proc:")
  (display (vector-ref table equiv-proc))
  (newline)
  (display "vector:")
  (display (vector-ref table hash-vector))
  (newline)
  (display "entries:")
  (display (hashtable-size table))
  (newline)
  (display "load-factor:")
  (display (load-factor table))
  (newline)
  )


;; call the appropriate hash/equiv proc
(define (do-hash table key)
  (let ((proc (vector-ref table hash-proc)))
    (cons (proc key) key)))

(define (do-entry table key obj) 
  (cons (do-hash table key) obj))

;; execute the equality operation
(define (do-equiv table key entry)
  ((vector-ref table equiv-proc) (get-key  key) (get-key entry)))


;; Accessors for an entry
(define (get-hash entry)
  (caar entry))

(define (get-key entry)
  (cdar entry))

(define (get-value entry) 
  (cadr entry))

;; mask off the part of the hash we don't need
(define (mask-hash table hash)
  (remainder hash (vector-length (vector-ref table hash-vector))))

;; find the location of a specific entry 
;; in a chain of entries
(define (find-entry table chain key)

  ;; loops over all entries in a given entry
  (define (inner size index)
    (cond
     ((<= size index) ;; we found nothing 
      '())
     ((do-equiv table key (vector-ref chain index))
      index)
     (else (inner size (+ 1 index)))))

  ;; check for empty cell
  (if (vector? chain)
      (inner (vector-length chain) 0)
      '()))

(define (inc-entries table) 
  (vector-set! table hash-entries
               (+ (vector-ref table hash-entries) 1)))

(define (dec-entries table) 
  (vector-set! table hash-entries
               (- (vector-ref table hash-entries) 1)))


;; Walk all of the ((hash . key) . value) pairs
(define (walk-entries table)

  ;; walk every element in this cell
  (define (walk-cell vector max index pairs)
    (if (>= index max)
        pairs
        (walk-cell vector
                   max
                   (+ 1 index) 
                   (cons (vector-ref vector index) pairs))))

  ;; walk every cell of the hash array
  (define (inner-table vector max index pairs)
    (cond 
     ((>= index max)
      pairs)
     ((null? (vector-ref vector index))
      (inner-table vector max (+ index 1) pairs))

     (else 
      (inner-table vector max (+ index 1)
                   (let* ((cell-vector (vector-ref vector index))
                          (cell-length (vector-length cell-vector)))
                     
                     (walk-cell cell-vector cell-length 0 pairs))))))

  ;; do the actual walk
  (let* ((vector (vector-ref table hash-vector))
         (max (vector-length vector)))
    (inner-table vector max 0 '())))

;; rehash the hash table to lower
;; the load-factor
(define (rehash table) 

  ;; calculate size baseb on what where we want the
  ;; load factor to be
  (define new-size 
    (floor (/ 
            (hashtable-size table) 
            max-load-factor)))
  
  (display "New-Size: ")
  (display new-size)
  (newline)

  ;; do we need to rehash?
  (if (> new-size (vector-length (vector-ref table hash-vector)))
      (begin (define new-vector
               (make-vector new-size))

             (define entries (walk-entries table))

             (display "REHASH")

             ;; zero things out and attach new table
             (vector-set! table hash-vector new-vector)
             (vector-set! table hash-entries 0)

             (for-each (lambda (entry)
                         (hashtable-write-entry table entry)) entries)
             (hash-info table)
             ))

  ;; now we need to walk entries and write them back into the hash-table
)


;; look up the value bound to a particular
;; key
(define (hashtable-ref table key default)
  (define entry (do-entry table key default))
  (define search (mask-hash table (get-hash entry)))
  (define cell (vector-ref (vector-ref table hash-vector) search))
  (define entry-index (find-entry table cell entry))
  
  (if (null? entry-index) 
      default
      (vector-ref cell entry-index)))

;; add an entry '((hash . key) . value)' to the hashtable 
(define (hashtable-write-entry table entry)
  (define search (mask-hash table (get-hash entry)))
  (define cell (vector-ref (vector-ref table hash-vector) search))
  (define entry-index (find-entry table cell entry))

  ;; deal with the set
  (cond 
   ((null? cell) ;; empty cell
    (begin
      ;; add new entry
      (inc-entries table)
      (vector-set! (vector-ref table hash-vector) search (vector entry))
      ;; do we need to rehash?
      (if (< max-load-factor (load-factor table))
          (rehash table))))

   ((null? entry-index) ;; new value
    (begin
      (inc-entries table)
      ;; do we need to resize the array?
      (if (< max-load-factor (load-factor table))
          (rehash table))
      (vector-set! (vector-ref table hash-vector) search 
                   (list->vector 
                    (cons 
                     entry
                     (vector->list cell))))))
   (else ;; existing entry
    (vector-set! cell entry-index entry))))

;; Add/set a key in the hashtable
(define (hashtable-set! table key obj)
  
  ;; setup the entry and search key
  ;; It would be better to do this with letrec (I think?)x
  (define entry (do-entry table key obj))
  (hashtable-write-entry table entry))

;; return the size of the table
(define (hashtable-size table) 
  (vector-ref table hash-entries))


(define (test-key table key)
  (display key)
  (display " => ")
  (display (hashtable-ref table key '()))
  (newline))

;; create a very crude test case
(define (test-hash) 
  (let ((table (make-eqv-hashtable)))
    (hashtable-set! table "h" 1)
    (hashtable-set! table "bad" 100)
    (hashtable-set! table "z" 2)
    (hashtable-set! table "e" 3)
    (hashtable-set! table "f" 4)
    (hashtable-set! table "g" 5)
    (hashtable-set! table "z" 50)
    (hashtable-set! table "h" 10)
    (hashtable-set! table "f" 5)
    (hash-info table)
    
    (test-key table "h")
    (test-key table "z")
    (test-key table "e")
    (test-key table "f")
    (test-key table "g")
    (test-key table "none")
    (test-key table "bad")
    ))

#t
