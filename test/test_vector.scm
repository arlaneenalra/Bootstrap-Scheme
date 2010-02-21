
(require "lib/test.scm")

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

   ))
