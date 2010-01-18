
(define Y
  (lambda (f)
    ((lambda (x) (f (lambda (y) ((x x) y))))
     (lambda (x) (f (lambda (y) ((x x) y)))))))


(define factorialy
  (Y (lambda (fact) 
       (lambda (n) 
	 (if (= n 0)
	     1
	     (* n (fact (- n 1))))))))


(factorialy 4)


(factorialy 5)

y
n


;(quit)
