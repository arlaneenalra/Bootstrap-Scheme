
(provide number? zero?)

;; returns true if the object is a number and 
;; false otherwise
(define (number? obj)
  (or (integer? obj)
      (real? obj)))


;; returns true if the given number is numerically equal to zero
(define (zero? num)
  (or 
   (and (real? num)
	(= 0.0 num))
   
   (and (integer? num)
	(= 0 num))))