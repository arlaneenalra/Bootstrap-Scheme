(dump_env)

(define Y
  (lambda (f)
    ((lambda (x) (f (lambda (y) ((x x) y))))
     (lambda (x) (f (lambda (y) ((x x) y)))))))

(dump_env)

(define factorialy
  (Y (lambda (fact) 
       (lambda (n) 
	 (if (= n 0)
	     1
	     (* n (fact (- n 1))))))))

fact

(dump_env)

(factorialy 4)

(dump_env)

(factorialy 5)

(dump_env)


(quit)
