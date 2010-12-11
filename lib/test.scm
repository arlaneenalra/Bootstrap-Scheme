;; Define functions for use in testing core functionality

(provide test)


;; A simple exception handler
(define (test-exception-handler exception)
  (display "An Exception occurred: ")
  (write exception)
  #f)

;; Test a single test case and check for the expected result
(define (test-case test-proc expected)
  (define actual '())

  ;; execute the test procedure
  (set! actual 
        (with-exception-handler test-exception-handler test-proc))


  (if (equal? actual expected)
      #t
      (begin
	(display "expected->")
	(write expected)
	(display " recieved->")
	(write actual)
	#f)))

;; run a list of tests
(define (test list)
  
  (define failed #f)

  (for-each 
   
   (lambda (tuple)
     (let
	 ((test-proc (car tuple))
	  (expected (cdr tuple)))

       (display "proc->")
       (write test-proc)
       (newline)
       
       (display "expected->")
       (write expected)
       (newline)
       

       (if (test-case test-proc expected)
	   (display "PASS")
	   (begin
	     (set! failed #t)
	     (display "FAIL")))
       (newline)))
   
   list)
  (if failed
      (error "Test failures")))
  
#t