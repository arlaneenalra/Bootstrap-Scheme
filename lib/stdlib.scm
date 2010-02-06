; Define some rather basic operations in scheme

;; ; define cons in scheme
;; (define (cons x y)
;;   (define pair '( () . () ))
;;   (set-car! pair x)
;;   (set-cdr! pair y)
;;   pair)

; load the string code library

(load "lib/string.scm")


; Display a new line
(define (newline)
  (write-char #\newline))

; output a string to the current default output
; port
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


; replaces ~a with what ever obj is as a string
(define (emit-inner template obj)

  (define obj-str
    (emit-interp obj))
  
  
  ;; (define output
  ;;    (make-string 
  ;;     (+ (- (string-length template) 2)
  ;; 	 (string-length obj-str))))


  
  (emit-string obj-str))


; Handle both forms of emit
(define (emit . args)
  (cond
   ((null? args) 
    (emit-string "Requires at least one argument"))
   ((null? (cdr args))
    (emit-inner (car args) ""))
   (else
    (emit-inner (car args) (cadr args)))))
		