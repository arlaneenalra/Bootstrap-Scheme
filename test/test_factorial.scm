(define (factorial n)
  (define (iter product counter max-count)
    (dump_env)
    (if (> counter max-count)
	product
	(iter (* counter product)
	      (+ counter 1)
	      max-count)))
  (iter 1 1 n))

iter

factorial 

(factorial 4)
;(factorial 5)

;(dump_env)

(quit)


