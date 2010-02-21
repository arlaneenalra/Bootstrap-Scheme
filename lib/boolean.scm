;; Define some boolean operators

(provide not boolean=? string=? equal? equal-seen)

(define (not x)
  (if x #f #t))


;; Check to see that both arguments are booleans
;; and equal
(define (boolean=? x y)
  (and (boolean? x) (boolean? y) (eq? x y)))

;; Check to see that both arguments are strings
;; and equal
;; TODO: This needs to be rewritten as scheme
(define (string=? x y)
  (and (string? x) (string? y) (eq? x y)))


;; break cycles in equal-pair?
;; TODO: This is very inefficient but will work
(define (equal-seen x seen-x)
  (cond
   ((null? seen-x) #f)
   ((eq? x (car seen-x)) #t)
   (else (equal-seen x (cdr seen-x)))))
      
	      


;; walk each the tree of pairs
;; TODO: This will have to change to deal with cycles
(define (equal-pair? x y)
  
  (define (equal-inner x y seen-x seen-y)
    (cond 
     ((and (equal-seen x seen-x)
	   (equal-seen y seen-y)) #t) ;; We have a cyle

     ((and (null? x) (null? y)) #t) ;; we have two nulls

     (else (and
	    (if (and (pair? (car x)) (pair? (car y)))
		(equal-inner (car x)
			     (car y)
			     (cons x seen-x) 
			     (cons y seen-y))
		(equal? (car x) (car y)))

	    (if (and (pair? (cdr x)) (pair? (cdr y)))
		(equal-inner (cdr x)
			     (cdr y)
			     (cons x seen-x) 
			     (cons y seen-y))
		(equal? (cdr x) (cdr y)))))))

  (equal-inner x y '() '()))
		    
		    
;; Check a pair of vectors for equality 
(define (equal-vector? x y)
  (let ((x (vector->list x))
	(y (vector->list y)))
    (equal-pair? x y)))
	   
  
;; Deep equality checking
(define (equal? x y)
  (cond
    ((and (boolean? x) (boolean? y)) (boolean=? x y))
    ((and (string? x) (string? y)) (string=? x y))
    ((and (pair? x) (pair? y)) (equal-pair? x y))
    ((and (vector? x) (vector? y)) (equal-vector? x y))

    ;; if we get here, things are not equal
    (else 
     (begin
       (eqv? x y)))))


#t
