
;; load our test library

(require "lib/test.scm")


(test

 '(((lambda () (and)) . #t)
   ((lambda () (and #f)) . #f)
   ((lambda () (and #t #t #t)) . #t)
   ((lambda () (and #t #f #t)) . #f)
   ((lambda () (and #t 2 #t)) . #t)

   ((lambda () (or)) . #f)
   ((lambda () (or #f)) . #f)
   ((lambda () (or #t #t #t)) . #t)
   ((lambda () (or #t #f #t)) . #t)
   ((lambda () (or #t 2 #t)) . #t)))


