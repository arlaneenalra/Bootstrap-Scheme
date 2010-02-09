; convert a string to characters

; load string functions that have been implemented 
; in scheme
(load "lib/stdlib.scm")

(load "lib/boolean.scm")
(load "lib/string.scm")
(load "lib/car_cdr.scm")


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



(substring "123456789" 0 4)
(substring "123456789" 5 6)


(string-one-append "This is" " a test")
(string-one-append "This is another " " a test")
(string-one-append "" "Testing")
(string-one-append "Testing" "")



; let's see how long a string we can make
(define numbers '())


(define massalloc
  (lambda (num)
    (+ 1 1 )
    (set! numbers
	  (cons 
	   (number->string num) numbers))
    (if (= num 0)
	numbers
	(massalloc (- num 1)))))

(massalloc 50)

;; (string->list numbers)


(string-append "1" "2" "3")
(join ":" "1" "2" "3")

(apply string-append numbers)

(define number-string
  (apply join ":" numbers))

number-string

(split ":" number-string)


(match '(#\c) '(#\c #\d #\d #\c))

(split "~a" "~a12 ~3 ~a testing ~aasd")
(split "~a" "~a12 ~3 ~a~a~a testing ~aasd")
(split "~a" " ~a12 ~3 ~a testing ~aasd")
(split "~" "~a12 ~3 ~~~~a testing ~aasd")
(split "" "12345")

(split "~a " "12 ~3 ~a testing ~aasd")

