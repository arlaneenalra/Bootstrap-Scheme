
(provide number? zero? fixnum->floatnum)

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

;; return a flotnum from a fix num
(define (fixnum->floatnum num) 
  (+ 0.0 num))


#t