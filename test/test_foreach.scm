
;; load our test library

(require "lib/test.scm")

(define val-list '())


(for-each 
 (lambda (x)
   (set! val-list
	 (cons (+ x x) val-list)))
 '(1 2 3 4 5 6 7 8 9 10))


;; (if (test (lambda () val-list) '(2 4 6 8 10 12 14 16 18 20))
;;     (display "GOOD")
;;     (display "FAIL"))


(test
 '(( (lambda () val-list) . '(2 4 6 8 10 12 14 16 18 20))))

(newline)



