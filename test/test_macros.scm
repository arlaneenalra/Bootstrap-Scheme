
(require "lib/test.scm")

(define-macro (do-nothing x) x)
(define-macro (make-lambda x) (lambda () x))

(test-suite
 (make-test ((do-nothing x)) x)
;; (make-test (make-lambda x) x)
;; (make-test (make-lambda #f) #f)
 )

