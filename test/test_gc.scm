; allocate a large number of objects

(define numbers '())
;(cons (number->string 1) numbers)
;(quit)
(define massalloc
  (lambda (num)
    (+ 1 1 )
    (set! numbers (cons (number->string num) numbers))
    (if (= num 0)
	numbers
	(massalloc (- num 1)))))

(massalloc 10)

(quit)