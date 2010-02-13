
(provide number?)

;; returns true if the object is a number and 
;; false otherwise
(define (number? obj)
  (or (integer? obj)
      (real? obj)))
