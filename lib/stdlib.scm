;; Define some rather basic operations in scheme

;; (load "lib/boolean.scm")

;; (load "lib/string.scm")

;; (load "lib/car_cdr.scm")


;; Loads the given module and allows exported symbols to be moved into
;; the passed in environment
(define (import-to-environment filename export-environment)
  
  (load filename)
  
  ;;(write  exported-list)
  
  ;; Mangle our list of variable names into a list of 
  ;; individual define statements hopefully without polluting
  ;; the libraries namespace
  (let ((inner 
	 (lambda (self exported-list)
	   (if (null? exported-list)
	       #t
	       (begin
		 ;; export to the passed environment
		 (eval (cons 'define
			     (cons (car exported-list)
				   (cons (eval (car exported-list)) '() )))
		       export-environment)
		 (self
		  self
		  (cdr exported-list)))))))

    (inner inner exported-list ))
  )


;; load strings library
(import-to-environment "lib/list.scm" (interaction-environment))
(import-to-environment "lib/string.scm" (interaction-environment))
(import-to-environment "lib/boolean.scm" (interaction-environment))
(import-to-environment "lib/car_cdr.scm" (interaction-environment))


