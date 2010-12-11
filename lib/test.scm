;; Define functions for use in testing core functionality

(provide make-test test-suite)

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

;; construct a test case 
(define-macro (make-test test-proc result)

  (cons 
   (cons 'lambda
         (cons '()
               test-proc))
   result))

;; iterate over tests and attempt to
;; execute them
(define (test-suite . tests)

  (newline)
  (display "Suite: ")
  (write tests)
  (newline)

  (test tests))
  
  ;; ;; Are we at the end of the suite?
  ;; (if (null? tests)
  ;;     #t
  ;;     (let*
  ;;         ((test-pair (car tests))
  ;;          (test-proc (car test-pair))
  ;;          (expected  (cdr test-pair)))
        
  ;;       ;; (display "proc->")
  ;;       ;; (write test-proc)
  ;;       ;; (newline)
        
  ;;       ;; (display "expected->")
  ;;       ;; (write expected)
  ;;       ;; (newline)

        
  ;;       (and 
  ;;        (if (test-case test-proc expected) ;; execute the test case
  ;;            (begin
  ;;              (display "PASS")
  ;;              #t)
  ;;            (begin
  ;;              (display "FAIL")
  ;;              #f))
  ;;        (test-suite (cdr tests))))))
         

    
#t