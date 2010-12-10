
(require "lib/test.scm")

(define-macro (do-nothing x) x)
(define-macro (make-lambda x) (lambda () x))

(test 
 '(
   ((lambda () (do-nothing x)) . x)
   ((make-lambda x) . x)
   ((make-lambda #f) . #f)
   ))

