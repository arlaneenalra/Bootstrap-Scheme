
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



; extract a given substring by index
(define (substring str start end)
  (define (inner new-str index)
    (if (< index end)
	(begin
	  (string-set! new-str (- index start)
		       (string-ref str index))

	  (inner new-str (+ 1 index))

	  )
	new-str
	))

  (inner (make-string (- end start)) start))


; combine two strings
;; (define (string-one-append str1 str2)
;;   (define str1-len (string-length str1))
;;   (define str2-len (string-length str2))
  
;;   ; iterate indexes
;;   (define (str-iter str str-len)
;;     (define index -1)
;;     (lambda () 
;;       (set! index (+ 1 index))
;;       (if (>= index str-len)
;; 	  '()
;; 	    (string-ref str index))))

;;   (define str1-iter (str-iter str1 str1-len))
;;   (define str2-iter (str-iter str2 str2-len))
  
;;   (define new-str (make-string (+ str1-len str2-len)))
;;   (define index 0)
       
  
;;   ; loop through an interator
;;   (define (do iter)
;;     ((lambda (x)
;;     (if (null? x)
;; 	'()
;; 	(begin 
;; 	  (string-set! new-str index x)
;; 	  (set! index (+ 1 index))
;; 	  (do iter)))) (iter)))
    

;;   (do str1-iter)
;;   (do str2-iter)
;;   new-str
;; )


(define (string-one-append str1 str2)
  (define str1-list (string->list str1))
  (define str2-list (string->list str2))
  
  (define (attach list1)
    (if (null? (cdr list1))
	(set-cdr! list1 str2-list)
	(attach (cdr list1))))
  (attach str1-list)

  (list->string str1-list))
    
  
 


  