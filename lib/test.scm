;; Define functions for use in testing core functionality

(provide test)


;; Test a single test case and check for the expected result
(define (test test-case expected)
  (define actual '())
  
  (set! actual (test-case))
  (if (eq? actual expected)
      #t
      #f))

