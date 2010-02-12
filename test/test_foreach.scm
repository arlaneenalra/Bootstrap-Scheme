
(define val-list '())

;; a for-each routine
(define (for-each proc val-list)

  (define (inner proc val-list)
    (if (or (null? val-list) (null? (car val-list)))
	'()
	(begin
	  (proc (car val-list))
	  (inner proc (cdr val-list)))))

  (inner proc val-list))

(for-each 
 (lambda (x)
   (set! val-list
	 (cons (+ x x) val-list)))
 '(1 2 3 4 5 6 7 8 9 10))

val-list
