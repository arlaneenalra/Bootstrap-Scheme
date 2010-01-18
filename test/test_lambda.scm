(define simple
  (lambda () (+ 1 1 )))

(simple)


(define count
  (lambda (inc)
    (define n 0)
    (lambda ()
      (set! n (+ inc n))
      n)))

(define step
  (count 3))
     

count
step

(step)
(step)
(step)

;(count 3)

;(count 5)

(quit)
