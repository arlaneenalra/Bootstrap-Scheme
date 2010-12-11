
(require "lib/test.scm")

(test-suite
 ;; Exception
 (make-test ((with-exception-handler 
              (lambda (x) #t)
              (lambda () #f bad))) 
            #t)
 ;; Failure
 (make-test ((with-exception-handler 
              (lambda (x) #f)
              (lambda () #t)))
            #t))