(define (factorial n)
  (define (iter product counter max-count)
    (if (> counter max-count)
	product
	(iter (* counter product)
	      (+ counter 1)
	      max-count)))
  (iter 1 1 n))

(dump_env)

factorial

(factorial 4)
(factorial 5)

(quit)


