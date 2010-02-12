(define x 1)

(define (inner-bind y) (define x y) x)

(inner-bind 2)

x
(quit)
