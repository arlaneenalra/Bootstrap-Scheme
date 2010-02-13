

(define in (open-input-port "test/test_eyeball/test_lexer.scm"))


(read in)


(close-input-port in)

(display "here")


(load "test/test_eyeball/test_lexer.scm")

(display "here2")
