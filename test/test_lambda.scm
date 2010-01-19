(define simple
  (lambda () (+ 1 1 )))

(simple)

(dump_env)

(simple)
(dump_env)

(define count
  (lambda (inc)
    (define n 0)
    (lambda ()
      (set! n (+ inc n))
      n)))

(dump_env)

(define step
  (count 3))

(dump_env)     

(step)

(dump_env)

(step)
(step)

(dump_env)

;(count 3)

;(count 5)

(quit)
