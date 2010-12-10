;; make sure thel list functions are working

(require "lib/test.scm")


(test 
 '(
   ((lambda () (list? '())) . #t)
   ((lambda () (list? 1)) . #f)
   ((lambda () (list? '(1 . 2))) . #f)
   ((lambda () (list? '(1 2))) . #t)
   ))


