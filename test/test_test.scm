(require "lib/test.scm")

(define (true) #t)
(define (false) #f)

;; some simple test cases 
(test-suite 
 (make-test ((write "T") #t) #t)
 (make-test ((write "F") (false)) #f)
 (make-test ((write "t")) #t)
 (make-test ((write "t")) #t))

