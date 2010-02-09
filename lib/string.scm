

;; find the length of a list
(define (list-length list)
  (define (inner list len) 
    (if (null? list)
	len
	(inner (cdr list) (+ 1 len))))
  (inner list 0))

;; reverse a list
(define (reverse list)
  (define (inner list rev-list)
    (if (null? list)
	rev-list
	(inner (cdr list)
	       (cons (car list) rev-list))))
  (inner list '()))


;; convert a string into a list of characters 
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


;; convert a list of characters into a string 
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



;; extract a given substring by index
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



;; internal function that does combination
;; of two strings
(define (string-one-append str1 str2)
  (define str1-list (string->list str1))
  (define str2-list (string->list str2))
  
  (define (attach list1)
    (if (null? (cdr list1))
	(set-cdr! list1 str2-list)
	(attach (cdr list1))))

  (if (null? str1-list)
      (list->string str2-list)
      (begin
	(attach str1-list)
	(list->string str1-list))))


;; variadic string append
(define (string-append . str)
  (define (inner new-str str)
    (if (null? str)
	new-str
	(inner
	 (string-one-append new-str (car str))
	 (cdr str))))
  (inner "" str))


;; interleave str and every element of str-list
(define (join str . str-list)
  
  (define (inner new-str str-list)
    (if (null? str-list)
	new-str
	(inner
	 (string-append new-str str (car str-list))
	 (cdr str-list))))

  (inner (car str-list) (cdr str-list)))


;; convert our lists back into strings
(define (list-list->list-string list-of-lists)

  (define list-of-strings '())
  
  (define (inner list-of-lists)
    (if (null? list-of-lists)
	list-of-strings
	(begin
	  (set! list-of-strings
		(cons (list->string (car list-of-lists))
		      list-of-strings))
	  (inner (cdr list-of-lists)))))

  (inner list-of-lists))


;; compare two lists of objects and see if they
;; are the same
(define (match pattern-list char-list)
  (cond
   ;; we're at the end of the pattern, 
   ((null? pattern-list) char-list) ;; this should be true
   
   ;; we're at the end of the string,
   ;; but not the end of the pattern
   ((null? char-list) #f)

   ;; check next character
   ((eqv? (car pattern-list)
	  (car char-list))

    (match (cdr pattern-list)
	   (cdr char-list)))
   
   ;; no match
   (else #f)))


;; break a into a list of substrings using 
;; a pattern

(define (split pattern str)

  ;; convert the pattern into
  ;; a list of characters
  (define pattern-list
    (string->list pattern))

  (define str-list
    (string->list str))

  (define str-split '())
  
  ;; do the actual split work
  (define (split-inner str-list moved-list)

    (cond
     ;; End condition match
     ((and (null? str-list)
	   (not (null? moved-list)))
      (set! str-split 
      	    (cons  moved-list str-split)))

     ((null? str-list)
      '())

     
    ((match pattern-list (cdr str-list))
	(begin
 	  (set! str-split 
		(cons 
		 (cons 
		  (car str-list) moved-list) 
		       str-split))

	  (split-inner
	   (cdr str-list)
	   '())))

    (else (split-inner 
	   (cdr str-list) 
	   (cons (car str-list) moved-list)))))

  (define (strip-match list)
    (or (match pattern-list list) list))

  ;; the contents of a list of lists
  (define (reverse-list-list list)
    (define (inner list rev-list)
      (if (null? list)
	  rev-list
	  (cond
	   ((null? (car list))
	    (inner (cdr list)
		   (cons '() rev-list)))
	   (else 
	    (inner (cdr list) 
		   (cons 
		    (strip-match (reverse (car list))) rev-list))))))
    (reverse (inner list '())))



  ;; check to see if we have a match at the very 
  ;; start of the string with a non "" pattern

  ;; (if (and (match pattern-list str-list) 
  ;; 	   (not (eqv? pattern "")))
      
  ;;     (set! str-split
  ;; 	    (cons '() str-split))
  ;;     '())


  (split-inner str-list '())

  (list-list->list-string (reverse-list-list str-split)))
