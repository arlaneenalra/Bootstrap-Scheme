;; make sure thel list functions are working

(require "lib/test.scm")


(test-suite 
 (make-test ((list? '())) #t)
 (make-test ((list? 1)) #f)
 (make-test ((list? '(1 . 2))) #f)
 (make-test ((list? '(1 2))) #t))


