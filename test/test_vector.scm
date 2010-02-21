
(require "lib/test.scm")

;; first test case
(define val-list '())
(define val-list2 '())
(define val-list3 '())

(vector-for-each 
 (lambda (x)
   (set! val-list
	 (cons (+ x x) val-list)))
 '#(1 2 3 4 5 6 7 8 9 10))

(vector-for-each 
 (lambda (x y)
   (set! val-list2
	 (cons (+ x y) val-list2)))
 '#(1 2 3 4 5 6 7 8 9 10)
 '#(2 3 4 5 6 7 8 9 10 11))

(vector-for-each 
 (lambda (x y z)
   (set! val-list3
	 (cons (+ x y z) val-list3)))
 '#(1 2 3 4 5 6 7 8 9 10)
 '#(2 3 4 5 6 7 8 9 10 11)
 '#(1 2 3 4 5 6 7 8 9 10))

(test
 '(((lambda () '#(1 2 3 4 5)) . #(1 2 3 4 5 ))
   ((lambda () '#(0 (2 2 2 2) "Anna")) . #(0 (2 2 2 2) "Anna"))

   ((lambda () (vector? '#(1 2 3 4))) . #t)
   ((lambda () (vector? '(1 2 3 4))) . #f)
   ((lambda () (vector? '#())) . #t)
   ((lambda () (vector? "testing")) . #f)

   ((lambda () (make-vector 3)) . #(() () ()))
   ((lambda () (make-vector 0)) . #())
   ((lambda () (make-vector 4 #\a)) . #(#\a #\a #\a #\a))

   ((lambda () (vector 4 #\b 'vector)) . #(4 #\b vector))
   ((lambda () (vector 'a 'b 'c)) . #(a b c))

   ((lambda () (vector-length '#(1 2 3 4))) . 4)
   ((lambda () (vector-length '#())) . 0)

   ((lambda () (vector-ref '#(1 1 2 3 5 8 13 21) 5)) . 8)

   ((lambda ()
      (let ((vec (vector 0 '(2 2 2 2) "Anna")))
	(vector-set! vec 1 '("Sue" "Sue"))
	vec)) . #(0 ("Sue" "Sue") "Anna"))

   ((lambda ()
      (vector->list '#(dah dah didah))) . (dah dah didah))

   ((lambda ()
      (list->vector '(dididit dah))) . #(dididit dah))
   
   ((lambda ()
      (let ((vec '#(1 2 3 4 5 )))
	(vector-fill! vec #\b)
	vec)) . #(#\b #\b #\b #\b #\b))

   ((lambda ()
      (let ((vec '#()))
   	(vector-fill! vec #\b)
   	vec)) . #())
   
   ;; for-each testing
   ((lambda () val-list) . (20 18 16 14 12 10 8 6 4 2))
   ((lambda () val-list2) . (21 19 17 15 13 11 9 7 5 3))
   ((lambda () val-list3) . (31 28 25 22 19 16 13 10 7 4))

   ;; map testing
   ((lambda () (vector-map 
		(lambda (x)
		  (+ x x))
		 '#(1 2 3 4 5 6 7 8 9 10))) . #(2 4 6 8 10 12 14 16 18 20))

   ((lambda () (vector-map 
		(lambda (x y)
		  (+ x y))
		 '#(1 2 3 4 5 6 7 8 9 10)
		 '#(2 3 4 5 6 7 8 9 10 11))) . #(3 5 7 9 11 13 15 17 19 21))

   ((lambda () (vector-map 
		(lambda (x y z)
		  (+ x y z))
		'#(1 2 3 4 5 6 7 8 9 10)
		'#(2 3 4 5 6 7 8 9 10 11)
		'#(1 2 3 4 5 6 7 8 9 10))) . #(4 7 10 13 16 19 22 25 28 31))

   ((lambda () (vector-map 
		cadr
		'#((a b) (d e) (g h)))) . #(b e h))

   ((lambda () (vector-map 
		+
		'#(1 2 3) '#(4 5 6))) . #(5 7 9))

   ((lambda () 
      (let ((count 0))
	(vector-map (lambda (ignored)
	       (set! count (+ count 1))
	       count)
	     '#(a b)))) . #(1 2))
   ))
