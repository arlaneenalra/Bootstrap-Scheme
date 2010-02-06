
; find the length of a list
(define (list-length list)
  (define (inner list len) 
    (if (null? list)
	len
	(inner (cdr list) (+ 1 len))))
  (inner list 0))


; convert a string into a list of characters 
(define (string->list str)
  (define inner-list '())
  
  (define (inner str len)
    (if (< len 0)
	inner-list
	(begin
	  (set! inner-list
		(cons (string-ref str len)
		      inner-list))
	  (inner str (- len 1))
	  )
	)
    )
  (inner str (- (string-length str) 1)))


; convert a list of characters into a string 
(define (list->string list) 
  (define (inner str list index)
    (if (null? list)
	str
	(begin
	  (string-set! str index (car list))
	  (inner str (cdr list) (+ 1 index))
	  ))
    )
  (inner (make-string (list-length list)) list 0))