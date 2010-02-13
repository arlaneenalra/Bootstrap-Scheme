
;; load our test library

(require "lib/test.scm")

(define val-list '())


(for-each 
 (lambda (x)
   (set! val-list
	 (cons (+ x x) val-list)))
 '(1 2 3 4 5 6 7 8 9 10))



(test
 '(( (lambda () val-list) . (20 18 16 14 12 10 8 6 4 2))))

(newline)



