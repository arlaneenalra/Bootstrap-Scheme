
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

(test-suite
 (make-test ('#(1 2 3 4 5)) #(1 2 3 4 5 ))
 (make-test ('#(0 (2 2 2 2) "Anna")) #(0 (2 2 2 2) "Anna"))
 
 (make-test ((vector? '#(1 2 3 4))) #t)
 (make-test ((vector? '(1 2 3 4))) #f)
 (make-test ((vector? '#())) #t)
 (make-test ((vector? "testing")) #f)

 (make-test ((make-vector 3)) #(() () ()))
 (make-test ((make-vector 0)) #())
 (make-test ((make-vector 4 #\a)) #(#\a #\a #\a #\a))

 (make-test ((vector 4 #\b 'vector)) #(4 #\b vector))
 (make-test ((vector 'a 'b 'c)) #(a b c))

 (make-test ((vector-length '#(1 2 3 4))) 4)
 (make-test ((vector-length '#())) 0)

 (make-test ((vector-ref '#(1 1 2 3 5 8 13 21) 5)) 8)

 (make-test (
    (let ((vec (vector 0 '(2 2 2 2) "Anna")))
      (vector-set! vec 1 '("Sue" "Sue"))
      vec)) #(0 ("Sue" "Sue") "Anna"))

 (make-test (
    (vector->list '#(dah dah didah))) (dah dah didah))

 (make-test (
    (list->vector '(dididit dah))) #(dididit dah))
 
 (make-test (
    (let ((vec '#(1 2 3 4 5 )))
      (vector-fill! vec #\b)
      vec)) #(#\b #\b #\b #\b #\b))

 (make-test (
    (let ((vec '#()))
      (vector-fill! vec #\b)
      vec)) #())
 
 ;; for-each testing
 (make-test (val-list) (20 18 16 14 12 10 8 6 4 2))
 (make-test (val-list2) (21 19 17 15 13 11 9 7 5 3))
 (make-test (val-list3) (31 28 25 22 19 16 13 10 7 4))

 ;; map testing
 (make-test ((vector-map 
              (lambda (x)
                (+ x x))
              '#(1 2 3 4 5 6 7 8 9 10))) #(2 4 6 8 10 12 14 16 18 20))

 (make-test ((vector-map 
              (lambda (x y)
                (+ x y))
              '#(1 2 3 4 5 6 7 8 9 10)
              '#(2 3 4 5 6 7 8 9 10 11))) #(3 5 7 9 11 13 15 17 19 21))

 (make-test ((vector-map 
              (lambda (x y z)
                (+ x y z))
              '#(1 2 3 4 5 6 7 8 9 10)
              '#(2 3 4 5 6 7 8 9 10 11)
              '#(1 2 3 4 5 6 7 8 9 10))) #(4 7 10 13 16 19 22 25 28 31))

 (make-test ((vector-map 
              cadr
              '#((a b) (d e) (g h)))) #(b e h))

 (make-test ((vector-map 
              +
              '#(1 2 3) '#(4 5 6))) #(5 7 9))

 (make-test (
             (let ((count 0))
               (vector-map (lambda (ignored)
                             (set! count (+ count 1))
                             count)
                           '#(a b)))) #(1 2)))
