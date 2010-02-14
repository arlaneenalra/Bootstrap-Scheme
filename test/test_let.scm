
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

    ((lambda ()
       (let ((x 2)
	     (y 3))
	 (let ((x 7)
	       (z (+ x y)))
	   (* x z)))) . 35)

    ((lambda ()
       (let ((x 2)
	     (y 3))
	 (let* ((x 7)
	       (z (+ x y)))
	   (* x z)))) . 70)
    
))

