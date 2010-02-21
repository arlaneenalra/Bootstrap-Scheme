
;; load our test library

(require "lib/test.scm")

(test
 '(((lambda () (map 
		(lambda (x)
		  (+ x x))
		 '(1 2 3 4 5 6 7 8 9 10))) . (2 4 6 8 10 12 14 16 18 20))

   ((lambda () (map 
		(lambda (x y)
		  (+ x y))
		 '(1 2 3 4 5 6 7 8 9 10)
		 '(2 3 4 5 6 7 8 9 10 11))) . (3 5 7 9 11 13 15 17 19 21))

   ((lambda () (map 
		(lambda (x y z)
		  (+ x y z))
		'(1 2 3 4 5 6 7 8 9 10)
		'(2 3 4 5 6 7 8 9 10 11)
		'(1 2 3 4 5 6 7 8 9 10))) . (4 7 10 13 16 19 22 25 28 31))

   ((lambda () (map 
		cadr
		'((a b) (d e) (g h)))) . (b e h))

   ((lambda () (map 
		+
		'(1 2 3) '(4 5 6))) . (5 7 9))

   ((lambda () 
      (let ((count 0))
	(map (lambda (ignored)
	       (set! count (+ count 1))
	       count)
	     '(a b)))) . (1 2))
			

))

