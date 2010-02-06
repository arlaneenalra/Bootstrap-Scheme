; convert a string to characters

; load string functions that have been implemented 
; in scheme
(load "lib/string.scm")


(make-string 10 #\c)

(make-string 10 )

(make-string 10 #\a)


(define working-string (make-string 10))

(string-length working-string)

(set! working-string (make-string 10 #\c))

(string-length working-string)

working-string

(string-set! working-string 1 #\a)

working-string

(string-ref working-string 1)
(string-ref working-string 0)

(string->list "This is a test")

(list-length (string->list "This was a test"))
(list->string (string->list "This was a test"))



