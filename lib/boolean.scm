;; Define some boolean operators

(provide not boolean=? string=? equal?)

(define (not x)
  (if x #f #t))


;; Check to see that both arguments are booleans
;; and equal
(define (boolean=? x y)
  (and (boolean? x) (boolean? y) (eq? x y)))

;; Check to see that both arguments are strings
;; and equal
(define (string=? x y)
  (and (string? x) (string? y) (eq? x y)))


;; walk each the tree of pairs
;; TODO: This will have to change to deal with cycles
(define (equal-pair? x y)
  (and (equal? (car x) (car y)) (equal? (cdr x) (cdr y))))

;; Deep equality checking
(define (equal? x y)
  (cond
    ((and (boolean? x) (boolean? y)) (boolean=? x y))
    ((and (string? x) (string? y)) (string=? x y))
    ((and (pair? x) (pair? y)) (equal-pair? x y))

    ;; if we get here, things are not equal
    (else (eqv? x y))))
