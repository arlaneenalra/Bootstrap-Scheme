

(define a '(1 2 3 4))
(define b '(1 2 3 4))
(define c '(1 2 3 ))

(define abc (cons a (cons b (cons c '()))))
(define ABC (cons a (cons b (cons c '()))))
(define acb (cons a (cons c (cons b '()))))

;; Construct a cycle 
(define (make-cycle)
  (define cycle (cons 1 (cons 2 (cons 3 (cons 4 (cons 5 '()))))))
  (define cycle-tail (cddddr cycle))
  (set-cdr! cycle-tail cycle)
  cycle)

(define cycle-a (make-cycle))
(define cycle-b (make-cycle))
(define cycle-c (make-cycle))

(let
    ((cycle-c-tail (cdddr cycle-c)))  
  (set-cdr! cycle-c-tail a))


(newline)

(pair? '())
(eq? (cons 1 2) (cons 1 2))
(eqv? (cons 1 1) '())

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


(equal? cycle-a cycle-a) ; #t
(equal? cycle-a cycle-b) ; #t
(equal? cycle-b cycle-a) ; #t
(equal? cycle-c cycle-a) ; #f
(equal? cycle-a cycle-c) ; #f

