
(require "lib/test.scm")

(test
 '(((lambda () '#(1 2 3 4 5)) . #(1 2 3 4 5 ))
   ((lambda () '#(0 (2 2 2 2) "Anna")) . #(0 (2 2 2 2) "Anna"))

   ((lambda () (vector? '#(1 2 3 4))) . #t)
   ((lambda () (vector? '(1 2 3 4))) . #f)
   ((lambda () (vector? '#())) . #t)
   ((lambda () (vector? "testing")) . #f)

   ((lambda () (vector 4 #\b 'vector)) . #(4 #\b vector))

   ((lambda () (vector-length '#(1 2 3 4))) . 4)
   ((lambda () (vector-length '#())) . 0)

   ((lambda () (make-vector 3)) . #(() () ()))
   ((lambda () (make-vector 0)) . #())
   ((lambda () (make-vector 4 #\a)) . #(#\a #\a #\a #\a))

   ))
