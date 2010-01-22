; An implementation of the newton method of finding square roots

; convenience
(define (square x) (* x x ))

; don't have this as a primitive yet
(define (>= x y) 
  (or 
   (> x y) 
   (= x y)))

; need to know absolute value
(define (abs x) 
  (if (< 0 x) 
      x
      (* -1 x)))

(define (diff guess x)
  (abs (- x (square guess))))

; Was the guess good enough for 
; Government work?
(define (good-enough? guess x)
  (if (> 0.001 (diff guess x))
      #t
      #f))

; find the average of two numbers 
; with a floating point result
(define (avg x y)
  ; my bootstrap requires the 2.0
  ; to do floating point math
  (/ (+ x y) 2.0))

; used to make the next guess 
(define (sqrt-improve guess x) 
  (avg (/ x guess) guess))


(define (sqrt-depth y)
  (define (sqrt-iter guess x depth)
    (if (good-enough? guess x)
	(list guess depth)
	(sqrt-iter (sqrt-improve guess x) x (+ 1 depth))))
  (sqrt-iter 1 y 1))

(define (sqrt y) 
  (car (sqrt-depth y)))

;(sqrt 2)
;(sqrt 4)


 
(sqrt 2)
(sqrt-depth 2)

(sqrt 5)
(sqrt-depth 5)

(sqrt 256)
(sqrt-depth 256)

(sqrt 1024)
(sqrt-depth 1024)

(sqrt 2048)
(sqrt-depth 2048)

(dump_env)

(quit)