

;(apply)
;(apply +)
;(apply + 1)
;(apply + 1 2 )

(define x '(1 2 3 4 ))

(apply + 1 2 3 '())
(apply + '(1 2 3 ))
(apply + 1 2 3 4 x)

(define args '(1 2 ))

args 
(apply cons '(1 2))
(apply cons args )

(define args '((1 . 2 )))
(apply car args)

(car (cons 1 2))

(quit)