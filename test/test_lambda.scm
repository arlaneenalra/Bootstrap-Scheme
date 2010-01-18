(define count
  	((lambda (total)
	   (lambda (increment)
	     (set! total (+ total increment))
	     total))
	   0))
count 
(count 3)

(count 5)

(define (a x) (if (= x 0) 0 (a (- x 1))))

(a 10)

(quit)
