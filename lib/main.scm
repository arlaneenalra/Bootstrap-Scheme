;; Setup the base system and boostrap the library loader

;;(load "lib/stdlib.scm")


;; definintions for a simple repl
;;(define env (interaction-environment))

;; (define (repl)
;;   (write-char #\])
;;   (write (eval (read) env))
;;   (write-char #\newline)
;;   (repl))

(define (i-e) (interaction-environment))

(define (spin x)
  (if (= 0 x)
      (cons 0 '())
      (cons x (spin (- x 1)))))

#t
