;; (define simple
;;   (lambda (n) (dump_env) (+ 1 1 n)))

;; (simple 1)

;; (dump_env)

(define count
  (lambda (inc)
    (define n 0)
    (lambda ()
      (dump_env)
      (set! n (+ inc n))
      n)))

(define step
  (count 3))

(define (jump n)
  (dump_env)
  (step)
  (if (= (- n 1) 0)
      (step)
      (jump (- n 1))))

(dump_env)     

(step)

(dump_env)

(step)
(step)

(dump_env)

(jump 10)

(quit)
