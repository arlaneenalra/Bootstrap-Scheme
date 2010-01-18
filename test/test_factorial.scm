(define (factorial n)
  (dump_env)
  (define (iter product counter max-count)
    (dump_env)
    (if (> counter max-count)
	product
	(iter (* counter product)
	      (+ counter 1)
	      max-count)))
  (dump_env)
  (iter 1 1 n))

(dump_env)

factorial

(factorial 4)
(factorial 5)

(dump_env)

(quit)


