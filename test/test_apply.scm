(require "lib/test.scm")

(define x '(1 2 3 4))

(define (sum . x)
  (define (inner x)
    (if (null? x)
        0
        (+ (car x) (inner (cdr x)))))
  (inner x))

(test
 '(
   ((lambda () (apply + 1 2 3 '())) . 6)
   ((lambda () (apply + '(1 2 3 ))) . 6)
   ((lambda () (apply + 1 2 3 4 x)) . 20)

   ((lambda () (apply + (list 3 4))) . 7) ;; Why is this failing?

   ((lambda () (apply cons '(1 2))) . (1 . 2))

   ((lambda () (let ((args '(1 2)))
                 (apply cons args))) . (1 . 2))

   ((lambda () (let
                   ((args '(1 2 3 4 5 6)))
                 (apply sum args))) . 21)

   ((lambda () (let
                   ((args '(1 2 3 4 5 6)))
                 (apply sum 1 2 3 4 5 6 args))) . 42)
   ))