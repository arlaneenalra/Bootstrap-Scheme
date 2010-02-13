

(provide newline display)


(define (newline) (write-char #\newline))

;; display a string character by 
;; character
(define (display-string obj port)
  (let
      ((char-list (string->list obj)))
    (for-each (lambda (char)
		(write-char char port))
	      char-list)))

(define (display-number obj port)
  (display-string 
   (number->string obj)
   port))

;; dispatch to the appropriate display method
(define (display-internal obj port)
  (cond
   ((string? obj) 
    (display-string obj port))

   ((char? obj) 
    (write-char obj port))
   
   ((integer? obj)
    (display-number obj port))

   (else 
    (write obj port))))



;; define a short proxy to use until I get the 
;; real io code implemented
(define (display . args)

  (if (= 2 (length args))
      (display-internal (car args) (cadr args))
      (display-internal (car args) (standard-output-port))))

#t