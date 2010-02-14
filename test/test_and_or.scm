
;; load our test library

(require "lib/test.scm")


(test

 '(((lambda () (and)) . #t)
   ((lambda () (and #f)) . #f)
   ((lambda () (and #t #t #t)) . #t)
   ((lambda () (and #t #f #t)) . #f)
   ((lambda () (and #t 2 #t)) . #t)

   ((lambda () (and (= 2 2) (> 2 1))) . #t)
   ((lambda () (and (= 2 2) (< 2 1))) . #f)
   ((lambda () (and 1 2 'c '(f g))) . (f g))

   ((lambda () (or)) . #f)
   ((lambda () (or #f)) . #f)
   ((lambda () (or #t #t #t)) . #t)
   ((lambda () (or #t #f #t)) . #t)
   ((lambda () (or #t 2 #t)) . #t)
   
   ((lambda () (or (= 2 2) (> 2 1))) . #t)
   ((lambda () (or (= 2 2) (< 2 1))) . #t)
   ((lambda () (or #f #f #f)) . #f)
   ((lambda () (or '(b c) (/ 3 0))) . (b c))))


