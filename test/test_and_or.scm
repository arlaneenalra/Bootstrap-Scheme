
;; load our test library

(require "lib/test.scm")


(test-suite
 (make-test ((and)) #t)
 (make-test ((and #f)) #f)
 (make-test ((and #t #t #t)) #t)
 (make-test ((and #t #f #t)) #f)
 (make-test ((and #t 2 #t)) #t)
 
 (make-test ((and (= 2 2) (> 2 1))) #t)
 (make-test ((and (= 2 2) (< 2 1))) #f)
 (make-test ((and 1 2 'c '(f g))) (f g))
 
 (make-test ((or)) #f)
 (make-test ((or #f)) #f)
 (make-test ((or #t #t #t)) #t)
 (make-test ((or #t #f #t)) #t)
 (make-test ((or #t 2 #t)) #t)
 
 (make-test ((or (= 2 2) (> 2 1))) #t)
 (make-test ((or (= 2 2) (< 2 1))) #t)
 (make-test ((or #f #f #f)) #f)
 (make-test ((or '(b c) (/ 3 0))) (b c)))


