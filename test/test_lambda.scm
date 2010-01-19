(define simple
  (lambda (n) (dump_env) (+ 1 1 n)))

(dump_env)

(simple 1)

(dump_env)

(simple 1)

(dump_env)

(quit)

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
