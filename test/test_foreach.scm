
;; load our test library

(require "lib/test.scm")

(define val-list '())
(define val-list2 '())
(define val-list3 '())

;; (prep-args 
;;  '((1 2 3 4 5 6 7 8 9 10)
;;    (a b c d e f g h i j)
;;    (1 2 3 4 5 6 7 8 9 10)
;;    (a b c d e f g h i j)))

;; (quit)


;; first test case
(for-each 
 (lambda (x)
   (set! val-list
	 (cons (+ x x) val-list)))
 '(1 2 3 4 5 6 7 8 9 10))


;; second test case
(for-each 
 (lambda (x y)
   (set! val-list2
	 (cons (+ x y) val-list2)))
 '(1 2 3 4 5 6 7 8 9 10)
 '(2 3 4 5 6 7 8 9 10 11))

;; third test case
(for-each 
 (lambda (x y z)
   (set! val-list3
	 (cons (+ x y z) val-list3)))
 '(1 2 3 4 5 6 7 8 9 10)
 '(2 3 4 5 6 7 8 9 10 11)
 '(1 2 3 4 5 6 7 8 9 10))


(test
 '(((lambda () val-list) . (20 18 16 14 12 10 8 6 4 2))
   ((lambda () val-list2) . (21 19 17 15 13 11 9 7 5 3))
   ((lambda () val-list3) . (31 28 25 22 19 16 13 10 7 4))))

(newline)
