
;; export some basic vector functions
(provide vector vector->list list->vector vector-fill! vector-for-each)


;; convert the given list into a vector
(define (list->vector list)
  (define vec 
    (make-vector (length list)))

  (define (inner list index)
    (if (null? list)
	vec
	(begin
	  (vector-set! vec index (car list))
	  (inner (cdr list) (+ 1 index)))))
  (inner list 0))


;; convert a vector to a list
(define (vector->list vec)
  (define (inner list index)
    (if (>= index (vector-length vec))
	(reverse list)
	(inner (cons (vector-ref vec index) list) (+ 1 index))))
  (inner '() 0))

;; Create a vector containing the passed in arguments
(define (vector . list)
  (list->vector list))


;; Fills a vector with the given object
(define (vector-fill! vec fill)
  (define (inner index)
    (if (>= index 0)
	(begin
	  (vector-set! vec index fill)
	  (inner (- index 1)))))
  (inner (- (vector-length vec) 1)))



;; for-each for vectors
(define (vector-for-each proc . vec-list)
  (define (inner vec-list new-args)
    (if (null? vec-list)
	(reverse new-args)
	(inner
	 (cdr vec-list)
	 (cons (vector->list (car vec-list)) new-args))))

  (apply for-each proc (inner vec-list '())))