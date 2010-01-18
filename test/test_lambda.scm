(define count
  	((lambda (total)
	   (lambda (increment)
	     (set! total (+ total increment))
	     total))
	   0))
count 
(count 3)

(count 5)
(quit)
