

(require "lib/test.scm")


(define test-val '(1 2 3 4 5 6 7 8 9 10))
(define test-val-car '((((((1 . 2) . 3) . 4) . 5) . 6) . 7))


(test-suite
 (make-test ((car test-val))  1)
 (make-test ((cdr test-val))  (2 3 4 5 6 7 8 9 10))

 (make-test ((caar test-val-car))  ((((1 . 2) . 3) . 4) . 5))
 (make-test ((cdar test-val-car)) 6)

 (make-test ((cadr test-val)) 2)
 (make-test ((cddr test-val)) (3 4 5 6 7 8 9 10))


 (make-test ((caaar test-val-car)) (((1 . 2) . 3) . 4))
 (make-test ((cdaar test-val-car)) 5)
 (make-test ((cadar '((#f . (#t . #f))  . #f))) #t)
 (make-test ((cddar '((1 2 3) . #f))) (3))

 (make-test ((caadr '(#f . ((#t . #f) . #f)))) #t)
 (make-test ((cdadr '(#f . ((#f . #t) . #f)))) #t)
 (make-test ((caddr '(#f . (#f . (#t . #f))))) #t)
 (make-test ((cdddr test-val)) (4 5 6 7 8 9 10))


 (make-test ((caaaar test-val-car)) ((1 . 2) . 3))
 (make-test ((cdaaar test-val-car))  4)
 (make-test ((cadaar '(((#f . (#t . #f))  . #f) . #f))) #t)
 (make-test ((cddaar '(((1 2 3) . #f) . #f))) (3))
 (make-test ((caadar '((#f . ((#t . #f) . #f)) . #f))) #t)
 (make-test ((cdadar '((#f . ((#f . #t) . #f)) . #f))) #t)
 (make-test ((caddar '((#f . (#f . (#t . #f))) . #f))) #t)
 (make-test ((cdddar '((1 2 3 4 5) . #f))) (4 5))

 (make-test ((caaadr '(#f . (((#t . #f) . #f) . #f)))) #t)
 (make-test ((cdaadr '(#f . (((#f . #t) . #f) . #f)))) #t)
 (make-test ((cadadr '(#f . ((#f . (#t . #f))  . #f)))) #t)
 (make-test ((cddadr '(#f . ((1 2 3) . #f)))) (3))
 (make-test ((caaddr '(#f . (#f . ((#t . #f) . #f))))) #t)
 (make-test ((cdaddr '(#f #f . ((#f . #t) . #f)))) #t)
 (make-test ((cadddr '(#f #f #f . (#t . #f)))) #t)
 (make-test ((cddddr test-val)) (5 6 7 8 9 10)))

   

