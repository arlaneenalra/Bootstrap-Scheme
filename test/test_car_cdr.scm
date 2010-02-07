

;;(define old-cadr cadr)
;;(eq? old-cadr cadr)

(load "lib/stdlib.scm")

;;(eq? old-cadr cadr)


(define test '(1 2 3 ))

(cadr test)
;;(old-cadr test)


;; (car-cdr-gen-proc (car car-cdr-list))
;; (car-cdr-gen-proc (car (cdr car-cdr-list)))