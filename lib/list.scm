
;; export some functions
(provide length reverse for-each map list?)

;; check to see if the current object 
;; is a list
(define (list? list)
  (cond
   ((equal? '() list) #t)
   ((pair? list) (list? (cdr list)))
   (else #f)))

;; find the length of a list
(define (length list)
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



;; convert parallel lists into a single list
;; of lists
(define (prep-args val-list)

  ;; make a list from the a row of the 
  ;; parallel lists
  (define (combind val-list flat-list)
    
    (if (null? val-list)
	(reverse flat-list)
	(combind (cdr val-list) 
		 (cons (car (car val-list)) flat-list))))

  (define (next val-list new-val-list)
    (cond 
     ((null? val-list) 
      (reverse new-val-list))
     
     ((null? (car val-list))
	     '())
     
     (else 
      (next (cdr val-list) 
	    (cons (cdr (car val-list)) new-val-list)))))

  
  (define (inner val-list args-list)
    (let
	((next-val (next val-list '())))

      (if (null? next-val)
	  (reverse args-list)
	  (inner next-val (cons  
			   (combind val-list '())
			   args-list)))))


  (inner val-list '()))
	
    

;; a for-each routine
(define (for-each proc . val-list)
  
  (define (inner val-list)
    (if (or (null? val-list) (null? (car val-list)))
	'()
	(begin
	  (apply proc (car val-list))
	  (inner (cdr val-list)))))

  (inner (prep-args val-list)))


;; map 
(define (map proc . val-list)

  (define (inner val-list result)
    (if (or (null? val-list) (null? (car val-list)))
	(reverse result)
	(inner (cdr val-list)
	       (cons 
		(apply proc (car val-list))
		result))))
	
  (inner (prep-args val-list) '()))
  

#t


