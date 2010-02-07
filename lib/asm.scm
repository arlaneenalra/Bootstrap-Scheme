; A simple code converter based on An Incremental Approach to Compiler Construction.

(load "lib/stdlib.scm")


;; Display a new line
(define (newline)
  (write-char #\newline))

;; output a string to the current default output
;; port
(define (emit-string str) 
  (define (inner index len)
    (if (< index len)
	(begin
	  (write-char (string-ref str index))
	  (inner (+ 1 index) len))))
  (inner 0 (string-length str)))



(define (emit-interp obj)
  (cond
   ((string? obj) obj)
   ((integer? obj) (number->string obj))
   ((char? obj) (write-char obj))
   (else (error "I don't know what this is"))
   ))


;; replaces ~a with what ever obj is as a string
(define (emit-inner template obj)

  (define obj-str
    (emit-interp obj))
  
  (define template-list
    (split "~a" template))
  
  (emit-string 
   (string-append (car template-list)
		  obj-str
		  (cadr template-list))))



;; Handle both forms of emit
(define (emit . args)
  (cond
   ((null? args) 
    (emit-string "Requires at least one argument"))

   ((null? (cdr args))
    (emit-string (emit-interp (car args))))

   (else
    (emit-inner (car args) (cadr args)))))



;; Actually do something
(begin
  (emit "this is ~a simple string" "Testing")
  (newline)
  (emit "This is a test")
  (newline))


