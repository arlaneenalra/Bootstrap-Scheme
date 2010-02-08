;; Tricks to define car and cdr down to appropriate depths


(define (car-cdr-iter-inner bits)
  (cond
   ((null? bits) #f)
   
   ((eqv? (car bits) #\a)
    (set-car! bits #\d))
   (else (begin
	   (set-car! bits #\a)
	   (car-cdr-iter-inner (cdr bits))))))

; iterate through a list of binary numbers out to the given max
(define (car-cdr-iter . bits)

  (set! bits (car bits))

  (define (clone bits result)
    (if (null? bits)
	result
	(clone (cdr bits)
	       (cons (car bits) result))))

  (lambda ()
    (if (inner bits)
	(clone bits '())
	#f)))
	    


;; walk every element in a list
(define (list-iter list)
  (define head list)
  
  (lambda ()
    (if (null? head)
	#f
	(let
	    ((temp head))
	  (set! head (cdr head))
	  (car temp)))))

;; loop through an iterator
(define (loop iter iteration result)
  (if iteration
      (loop iter (iter) (cons iteration result))
      result))


(define (car-cdr-loop start)
  (loop
   (car-cdr-iter start) start '()))


;; generate a list of character lists
;; that represents every combination of car and cdr
;; we want

(define car-cdr-list
  (let 
      ((iter  (list-iter (car-cdr-loop '(#\a #\a))))
       (iter2 (list-iter (car-cdr-loop '(#\a #\a #\a))))
       (iter3 (list-iter (car-cdr-loop '(#\a #\a #\a #\a)))))
    
    (loop iter (iter) 
	  (loop iter2 (iter2) 
		(loop iter3 (iter3) '())))))



;; construct a procedure define based on a
;; list of charactes
(define (car-cdr-gen-proc x)
  (define symbol
    (string->symbol 
     (string-append
      "c" (list->string x) "r")))
  
  ;; construct a list of symbols for the 
  ;; internals of our c?r proc
  (define call-list
    (loop
     (lambda ()
       (if (null? x)
	   #f
	   (begin 
	     (define next
	       ;; Make the symbol
	       (string->symbol
		(string-append "c" 
			       (list->string (cons (car x) '())) "r")))
	     ;; prune the list
	     (set! x (cdr x))
	     next)))
     '() '()))

  ;; construct actual S-Expressions for the body
  (define call 'param)

  (define (builder list)
    (if (null? (car list))
	call
	(begin
	  (set! call
		(cons (car list) 
		      (cons call '())))
	  (builder (cdr list)))))

  ;; construct the define
  (cons 'define
	(cons 
	 (cons symbol 
	       (cons 'param '()))
	 (cons
	  (builder call-list) '()))))


;; Evaluates the list of expressions in the given 
;; environment
(define (build-defines list env)
  (let
      ((proc (car-cdr-gen-proc (car list)))
       (next (cdr list)))
    (eval proc env)
    (if (null? next)
	#t
    (build-defines next env))))

;; build car-cdr friends
(build-defines car-cdr-list (interaction-environment))
