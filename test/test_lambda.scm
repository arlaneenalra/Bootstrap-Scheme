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

(quit)
