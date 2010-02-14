
(require "lib/test.scm")




(test
 '(
   ((lambda () 
      (let ((x 1) 
	    (y 2) 
	    (z 3)) 
	(+ x y) 
	(+ y z) 
	(+ x y z))) . 6)

    ((lambda () 
       (let ((x 2) 
	     (y 3)) 
	 (* x y))) . 6)

    ((lambda () ;; borrowed from the spec
       (let ((x 2)
	     (y 3))
	 (let ((x 7)
	       (z (+ x y)))
	   (* x z)))) . 35)

    ((lambda () ;; borrowed fromt he spec
       (let ((x 2)
	     (y 3))
	 (let* ((x 7)
	       (z (+ x y)))
	   (* x z)))) . 70)

    ((lambda () ;; borrowed from the spec
       (letrec ((even?
		 (lambda (n)
		   (if (zero? n)
		       #t
		       (odd? (- n 1)))))
		(odd?
		 (lambda (n)
		   (if (zero? n)
		       #t
		       (even? (- n 1))))))
	 (even? 88))) . #t)

    ((lambda () ;; borrowed from the spec
       (letrec* ((p
		  (lambda (x)
		    (+ 1 (q (- x 1)))))
		 (q
		  (lambda (y)
		    (if (zero? y)
			0
			(+ 1 (p (- y 1))))))
		 (x (p 5))
		 (y x))
		y)) . 5)

    
))

