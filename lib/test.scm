;; Define functions for use in testing core functionality

(provide test)


;; Test a single test case and check for the expected result
(define (test-case test-proc expected)
  (define actual '())

  (set! actual (test-proc))
  (if (equal? actual expected)
      #t
      (begin
	(display "expected->?")
	(write expected)
	(display " recieved->")
	(write actual)
	#f)))


;; run a list of tests
(define (test list)

  (for-each 

   (lambda (tuple)
     (let
	 ((test-proc (car tuple))
	  (expected (cdr tuple)))
       
       (if (test-case (eval test-proc) expected)
	   (display "PASS")
	   (display "FAIL"))
       (newline)))
   
   list))



