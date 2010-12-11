
(require "lib/test.scm")

(test 
 '(
   ;; Exception
   ((lambda () (with-exception-handler 
                (lambda (x) #t)
                (lambda () #f bad))) . #t)
   ;; Failure
   ((lambda () (with-exception-handler 
                (lambda (x) #f)
                (lambda () #t))) . #t)
   ))