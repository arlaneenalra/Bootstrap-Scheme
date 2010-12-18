

(provide string-hash make-hashtable make-eqv-hashtable hash-info hashtable-set! test-hash)

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

;; (define (make-eqv-hashtable)
;;   (make-hashtable string-hash eqv?))
(define make-eqv-hashtable 
  (lambda () (make-hashtable string-hash eqv?)))

  ;; accessors for a hashtable
  
(define (hash-proc table)
  (vector-ref table 0))

(define (equiv-proc table) 
  (vector-ref table 1))

(define (hash-vector table)
  (vector-ref table 2))

(define (hash-entries table)
  (vector-ref table 3))

(define (hash-info table)
  (newline)
  (display "hash-proc:")
  (display (hash-proc table))
  (newline)
  (display "equiv-proc:")
  (display (equiv-proc table))
  (newline)
  (display "vector:")
  (display (hash-vector table))
  (newline)
  (display "entries:")
  (display (hash-entries table))
  (newline)
  )


;; call the appropriate hash/equiv proc
(define (do-hash table key)
  (let ((proc (hash-proc table)))
    (cons (proc key) key)))

(define (do-entry table key obj) 
  (cons (do-hash table key) obj))

;; execute the equality operation
(define (do-equiv table key entry)
  ((equiv-proc table) (get-key  key) (get-key entry)))


;; Accessors for an entry
(define (get-hash entry)
  (caar entry))

(define (get-key entry)
  (cdar entry))

(define (get-value entry) 
  (cadr entry))

;; mask off the part of the hash we don't need
(define (mask-hash table hash)
  (remainder hash (vector-length (hash-vector table))))

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

;; look up the value bound to a particular
;; key
(define (hashtable-ref table key default)
  (define entry (do-entry table key default))
  (define search (mask-hash table (get-hash entry)))
  (define cell (vector-ref (hash-vector table) search))
  (define entry-index (find-entry table cell entry))
  
  (if (null? entry-index) 
      default
      (vector-ref cell entry-index)))


;; Add/set a key in the hashtable
(define (hashtable-set! table key obj)
  
  ;; setup the entry and search key
  ;; It would be better to do this with letrec (I think?)x
  (define entry (do-entry table key obj))
  (define search (mask-hash table (get-hash entry)))
  (define cell (vector-ref (hash-vector table) search))
  (define entry-index (find-entry table cell entry))


  (cond 
   ((null? cell) ;; empty cell
    (vector-set! (hash-vector table) search (vector entry)))

   ((null? entry-index) ;; new value
    (vector-set! (hash-vector table) search 
                 (list->vector 
                  (cons 
                   entry
                   (vector->list cell)))))
   (else ;; existing entry
    (vector-set! cell entry-index entry))))



(define (test-key table key)
  (display key)
  (display " => ")
  (display (hashtable-ref table key '()))
  (newline))

;; create a very crude test case
(define (test-hash) 
  (let ((table (make-eqv-hashtable)))
    (hashtable-set! table "h" 1)
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
    
    ))

#t
