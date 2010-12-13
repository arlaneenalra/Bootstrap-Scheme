
(require "lib/test.scm")

(define (handler e)
  #t)

(define (true) #t)
(define (false) #f)

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
            #t)
 
 (make-test ((with-exception-handler 
              handler
              (lambda () #f bad)))
            #t) 
 (make-test ((with-exception-handler 
              handler
              (lambda () #f)))
            #f)
)
