;; (define simple
;;   (lambda (n) (interaction-environment) (+ 1 1 n)))

;; (simple 1)

;; (interaction-environment)

(define count
  (lambda (inc)
    (define n 0)
    (lambda ()
      (interaction-environment)
      (set! n (+ inc n))
      n)))

(define step
  (count 3))

(define (jump n)
  (interaction-environment)
  (step)
  (if (= (- n 1) 0)
      (step)
      (jump (- n 1))))

(interaction-environment)     

(step)

(interaction-environment)

(step)
(step)

(interaction-environment)

(jump 10)


; test variadic lambda's
(define variadic
  (lambda x x))

variadic

(variadic 1 2 3 4 5 6 7)

(define variadic2
  (lambda (x y . z) 
    (cons x (cons y (cons z '())))))

variadic2

(variadic2 1 2 3 4 5 6)


(define (variadic3 . x)
  x)


variadic3

(variadic3 1 2 3 4 5 6 )

(define (variadic4 x y . z)
  (cons x (cons y (cons z '()))))

variadic4

(variadic4 1 2 3 4 5 6 )


(quit)
