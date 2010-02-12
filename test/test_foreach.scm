
(define val-list '())


(for-each 
 (lambda (x)
   (set! val-list
	 (cons (+ x x) val-list)))
 '(1 2 3 4 5 6 7 8 9 10))

val-list
