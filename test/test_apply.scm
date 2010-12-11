(require "lib/test.scm")

(define x '(1 2 3 4))

(define (sum . x)
  (define (inner x)
    (if (null? x)
        0
        (+ (car x) (inner (cdr x)))))
  (inner x))

(test-suite
   (make-test ((apply + 1 2 3 '())) 6)
   (make-test ((apply + '(1 2 3 ))) 6)
   (make-test ((apply + 1 2 3 4 x)) 20)

   (make-test ((apply + (list 3 4))) 7) 
 
   (make-test ((apply cons '(1 2))) (1 . 2))

   (make-test ((let ((args '(1 2)))
                  (apply cons args))) (1 . 2))

   (make-test ((let
                   ((args '(1 2 3 4 5 6)))
                 (apply sum args))) 21)

   (make-test ((let
                   ((args '(1 2 3 4 5 6)))
                 (apply sum 1 2 3 4 5 6 args))) 42))