;; A simple REPL

(define env (interaction-environment))

(define (handler e) 
  (display "An exception has occured: ")
  (display e)
  (newline))

;; a very simple repl
(define (repl)
  
  ;; display a prompt
  (write-char #\])
  
  (with-exception-handler 
   handler
   (write (eval (read) env)))
   
   (newline)
   (repl))
  
(display "We're metacircular baby!")
(newline)
(repl)