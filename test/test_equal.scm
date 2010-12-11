
(require "lib/test.scm")

(define a '(1 2 3 4))
(define b '(1 2 3 4))
(define c '(1 2 3 ))

(define vec-a '#(1 2 3 4))
(define vec-b '#(1 2 3 4))
(define vec-c '#(1 2 3))

(define abc (cons a (cons b (cons c '()))))
(define ABC (cons a (cons b (cons c '()))))
(define acb (cons a (cons c (cons b '()))))

;; Construct a cycle 
(define (make-cycle-cdr)
  (define cycle (cons 1 (cons 2 (cons 3 (cons 4 (cons 5 '()))))))
  (define cycle-tail (cddddr cycle))
  (set-cdr! cycle-tail cycle)
  cycle)

(define (make-cycle-car)
  (define cycle (cons (cons (cons (cons (cons 5 '()) 4) 3) 2) 1))
  (define cycle-tail (caaaar cycle))
  (set-car! cycle-tail cycle)
  cycle)

(define cycle-a (make-cycle-cdr))
(define cycle-b (make-cycle-cdr))
(define cycle-c (make-cycle-cdr))

(define cycle-a-a (make-cycle-car))
(define cycle-b-a (make-cycle-car))
(define cycle-c-a (make-cycle-car))

(let
    ((cycle-c-tail (cdddr cycle-c)))  
  (set-cdr! cycle-c-tail a))

(let
    ((cycle-c-tail (caaar cycle-c-a)))  
  (set-car! cycle-c-tail a))


(test-suite
 (make-test ((equal? '() '())) #t)
 (make-test ((eq? (cons 1 2) (cons 1 2))) #f)
 (make-test ((eqv? (cons 1 1) '())) #f)
 (make-test ((equal? a a)) #t)
 (make-test ((equal? a b)) #t)
 (make-test ((equal? b a)) #t)
 (make-test ((equal? c a)) #f)
 (make-test ((equal? a c)) #f)

 (make-test ((equal? abc abc)) #t)
 (make-test ((equal? abc ABC)) #t)
 (make-test ((equal? ABC abc)) #t)
 (make-test ((equal? acb abc)) #f)
 (make-test ((equal? abc acb)) #f)

 (make-test ((equal? abc a)) #f)

 (make-test ((equal? vec-a vec-a)) #t)
 (make-test ((equal? vec-a vec-b)) #t)
 (make-test ((equal? vec-b vec-a)) #t)
 (make-test ((equal? vec-c vec-a)) #f)
 (make-test ((equal? vec-a vec-c)) #f)

 (make-test ((equal? cycle-a cycle-a)) #t)
 (make-test ((equal? cycle-a cycle-b)) #t)
 (make-test ((equal? cycle-b cycle-a)) #t)
 (make-test ((equal? cycle-c cycle-a)) #f)
 (make-test ((equal? cycle-a cycle-c)) #f)

 (make-test ((equal? cycle-a-a cycle-a-a)) #t)
 (make-test ((equal? cycle-a-a cycle-b-a)) #t)
 (make-test ((equal? cycle-b-a cycle-a-a)) #t)
 (make-test ((equal? cycle-c-a cycle-a-a)) #f)
 (make-test ((equal? cycle-a-a cycle-c-a)) #f))


