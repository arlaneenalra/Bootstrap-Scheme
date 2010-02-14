

(require "lib/test.scm")


(define test-val '(1 2 3 4 5 6 7 8 9 10))
(define test-val-car '((((((1 . 2) . 3) . 4) . 5) . 6) . 7))


(test
 '(
   ((lambda ()
      (car test-val)) . 1)
   ((lambda ()
      (cdr test-val)) . (2 3 4 5 6 7 8 9 10))

   ((lambda ()
      (caar test-val-car)) . ((((1 . 2) . 3) . 4) . 5))
   ((lambda ()
      (cdar test-val-car)) . 6)

   ((lambda ()
      (cadr test-val)) . 2)
   ((lambda ()
      (cddr test-val)) . (3 4 5 6 7 8 9 10))


   ((lambda ()
      (caaar test-val-car)) . (((1 . 2) . 3) . 4))
   ((lambda ()
      (cdaar test-val-car)) . 5)
   ((lambda ()
      (cadar '((#f . (#t . #f))  . #f))) . #t)
   ((lambda ()
      (cddar '((1 2 3) . #f))) . (3))

   ((lambda ()
      (caadr '(#f . ((#t . #f) . #f)))) . #t)
   ((lambda ()
      (cdadr '(#f . ((#f . #t) . #f)))) . #t)
   ((lambda ()
      (caddr '(#f . (#f . (#t . #f))))) . #t)
   ((lambda ()
      (cdddr test-val)) . (4 5 6 7 8 9 10))


   ((lambda ()
      (caaaar test-val-car)) . ((1 . 2) . 3))
   ((lambda ()
      (cdaaar test-val-car)) . 4)
   ((lambda ()
      (cadaar '(((#f . (#t . #f))  . #f) . #f))) . #t)
   ((lambda ()
      (cddaar '(((1 2 3) . #f) . #f))) . (3))

   ((lambda ()
      (caadar '((#f . ((#t . #f) . #f)) . #f))) . #t)
   ((lambda ()
      (cdadar '((#f . ((#f . #t) . #f)) . #f))) . #t)
   ((lambda ()
      (caddar '((#f . (#f . (#t . #f))) . #f))) . #t)
   ((lambda ()
      (cdddar '((1 2 3 4 5) . #f))) . (4 5))

   ((lambda ()
      (caaadr '(#f . (((#t . #f) . #f) . #f)))) . #t)
   ((lambda ()
      (cdaadr '(#f . (((#f . #t) . #f) . #f)))) . #t)

   ((lambda ()
      (cadadr '(#f . ((#f . (#t . #f))  . #f)))) . #t)

   ((lambda ()
      (cddadr '(#f . ((1 2 3) . #f)))) . (3))

   ((lambda ()
      (caaddr '(#f . (#f . ((#t . #f) . #f))))) . #t)
   ((lambda ()
      (cdaddr '(#f #f . ((#f . #t) . #f)))) . #t)

   ((lambda ()
      (cadddr '(#f #f #f . (#t . #f)))) . #t)

   ((lambda ()
      (cddddr test-val)) . (5 6 7 8 9 10))))

   

