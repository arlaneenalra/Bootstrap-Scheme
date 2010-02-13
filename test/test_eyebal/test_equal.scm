

(define a '(1 2 3 4))
(define b '(1 2 3 4))
(define c '(1 2 3 ))

(define abc (cons a (cons b (cons c '()))))
(define ABC (cons a (cons b (cons c '()))))
(define acb (cons a (cons c (cons b '()))))


(newline)

(equal? a a) ; #t
(equal? a b) ; #t
(equal? b a) ; #t
(equal? c a) ; #f 
(equal? a c) ; #f

(newline)

(equal? abc abc) ; #t
(equal? abc ABC) ; #t
(equal? ABC abc) ; #t
(equal? acb abc) ; #f 
(equal? abc acb) ; #f

(newline)

(equal? abc a) ; #f


