
;; TODO: This approach will still lead to namespace corruption, I'll
;; need to come up with an eval based means to load into a private 
;; environment rather than the procedure one.  But this will require 
;; a more complete approach to libraries

;; Loads the given module and allows exported symbols to be moved into
;; the passed in environment
(define (import-to-environment filename export-environment)
  
  ;; Load a file into the lambda environment created with 
  ;; this function's closure.  So long as nothing changes global 
  ;; environment values this apporach should avoid poluting the 
  ;; global namespace with library names
  (load filename)
  
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




