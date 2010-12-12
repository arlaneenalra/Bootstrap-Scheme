;; A simple REPL

;; redefine the top-level environment
;;(define (interaction-environment) top-env)

(define (handler e) 
  (newline)
  (display "An exception has occured: ")
  (display e)
  (newline))


;; (define (env-setup env)
;;  (eval '(load "lib/stdlib.scm") top-env))

;; a very simple repl
(define (repl)  
  ;; display a prompt
  (write-char #\])

  (with-exception-handler 
   handler
   (lambda () 
     (write  (eval (read)))))

  (newline)
  
  (repl))

(display "We're metacircular baby!")
(newline)
(repl)