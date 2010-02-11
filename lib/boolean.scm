;; Define some boolean operators

(provide not)

(define (not x)
  (if x #f #t))
