

(define a '(1 2 3 4))
(define b '(1 2 3 4))
(define c '(1 2 3 ))

(newline)

(equal? a a) ; #t
(equal? a b) ; #t
(equal? b a) ; #t
(equal? c a) ; #f 
(equal? a c) ; #f
