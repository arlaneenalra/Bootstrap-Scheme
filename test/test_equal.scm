
(require "lib/test.scm")

(define a '(1 2 3 4))
(define b '(1 2 3 4))
(define c '(1 2 3 ))

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


(test
 '(((lambda () (eq? (cons 1 2) (cons 1 2))) . #f)
   ((lambda () (eqv? (cons 1 1) '())) . #f)
   ((lambda () (equal? a a)) . #t)
   ((lambda () (equal? a b)) . #t)
   ((lambda () (equal? b a)) . #t)
   ((lambda () (equal? c a)) . #f)
   ((lambda () (equal? a c)) . #f)

   ((lambda () (equal? abc abc)) . #t)
   ((lambda () (equal? abc ABC)) . #t)
   ((lambda () (equal? ABC abc)) . #t)
   ((lambda () (equal? acb abc)) . #f)
   ((lambda () (equal? abc acb)) . #f)

   ((lambda () (equal? abc a)) . #f)

   ((lambda () (equal? cycle-a cycle-a)) . #t)
   ((lambda () (equal? cycle-a cycle-b)) . #t)
   ((lambda () (equal? cycle-b cycle-a)) . #t)
   ((lambda () (equal? cycle-c cycle-a)) . #f)
   ((lambda () (equal? cycle-a cycle-c)) . #f)

   ((lambda () (equal? cycle-a-a cycle-a-a)) . #t)
   ((lambda () (equal? cycle-a-a cycle-b-a)) . #t)
   ((lambda () (equal? cycle-b-a cycle-a-a)) . #t)
   ((lambda () (equal? cycle-c-a cycle-a-a)) . #f)
   ((lambda () (equal? cycle-a-a cycle-c-a)) . #f)))


