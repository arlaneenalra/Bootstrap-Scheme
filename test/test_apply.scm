

;(apply)
;(apply +)
;(apply + 1)
;(apply + 1 2 )

(define x '(1 2 3 4 ))

(apply + 1 2 3 '())
(apply + '(1 2 3 ))
(apply + 1 2 3 4 x)
(apply + (list 1 2 3))

(define args '(1 2 ))

args 
(apply cons '(1 2))
(apply cons args )

(define args '((1 . 2 )))
(apply car args)


(car (cons 1 2))
(car '(1 . 2))


(define (sum . x)
  (define (inner x)
  (if (null? x)
      0
      (+ (car x) (inner (cdr x)))))
  (inner x))

(sum 1 2 3 4 5 6)

(define args '(1 2 3 4 5 6 ))

(apply sum args)

(quit)