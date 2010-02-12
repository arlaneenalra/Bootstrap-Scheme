(define (factorial n)
  (define (iter product counter max-count)
    (interaction-environment)
    (if (> counter max-count)
	product
	(iter (* counter product)
	      (+ counter 1)
	      max-count)))
  (iter 1 1 n))

(factorial 4)
;(factorial 5)

(interaction-environment)

(quit)


