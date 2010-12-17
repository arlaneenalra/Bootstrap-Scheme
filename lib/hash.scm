

(provide string-hash make-hashtable make-eqv-hashtable hash-info hashtable-set! test-hash)

;; a base size for the hash
(define hash-base-size 4)

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
  (list hash-proc equiv-proc (make-vector hash-base-size)))

;; (define (make-eqv-hashtable)
;;   (make-hashtable string-hash eqv?))
(define (make-eqv-hashtable) 
  (make-hashtable string-hash eqv?))

(define (hash-proc table)
  (car table))

(define (equiv-proc table) 
  (cadr table))

(define (hash-vector table)
  (caddr table))

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
  (newline))


;; call the appropriate hash/equiv proc
(define (do-hash table key)
  (let ((proc (hash-proc table)))
      (cons (proc key) key)))

(define (do-equiv proc key entry)
  (proc (get-key  key) (get-key entry)))

(define (get-hash entry)
  (caar entry))

(define (get-key entry)
  (cdar entry))

;; look up the value bound to a particular
;; key
;; (define (hashtable-ref table key default))

;; Add/set a key in the hashtable
(define (hashtable-set! table key obj)
  
  ;; setup the entry and search key
  ;; It would be better to do this with letrec (I think?)x
  (define entry (cons (do-hash table key) obj))
  (define search (remainder (get-hash entry) hash-base-size))
  (define cell (vector-ref (hash-vector table) search))
  
  (cond
   ((or
     (eqv? cell '())
     (do-equiv (equiv-proc table) entry cell))
    (begin 
      (vector-set! (hash-vector table) search entry)))
   (else #f)))

  ;; ;; check to see if the cell is set
  ;; (cond 
  ;;  ((or
  ;;    (eq? cell '())
  ;;    (do-equiv (equiv-proc table) cell entry))
  ;;   (begin 
  ;;     (write "SET")
  ;;     (vector-set! (hash-vector table) search entry)))
   
  ;;  (else
  ;;   #t)))


(define test-hash (make-eqv-hashtable))

(hashtable-set! test-hash "t" 1)
(hashtable-set! test-hash "f" 2)
(hashtable-set! test-hash "g" 3)
(hashtable-set! test-hash "h" 4)



#t
