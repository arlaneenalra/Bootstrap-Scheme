
;; export some functions
(provide length reverse for-each)

;; find the length of a list
(define (length list)
  (define (inner list len) 
    (if (null? list)
	len
	(inner (cdr list) (+ 1 len))))
  (inner list 0))

;; reverse a list
(define (reverse list)
  (define (inner list rev-list)
    (if (null? list)
	rev-list
	(inner (cdr list)
	       (cons (car list) rev-list))))
  (inner list '()))


;; a for-each routine
;; a for-each routine
(define (for-each proc val-list)

  (define (inner proc val-list)
    (if (or (null? val-list) (null? (car val-list)))
	'()
	(begin
	  (proc (car val-list))
	  (inner proc (cdr val-list)))))

  (inner proc val-list))

