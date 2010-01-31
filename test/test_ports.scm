; This must be the first line

; open the port
(define in-port (open-input-file "test/test_ports.scm"))

(define (invert char-list)
  (define (invert-internal char-list inverted-list)
    (if (eq? '() (cdr char-list))
	(cons (car char-list) inverted-list)
	(invert-internal (cdr char-list) (cons (car char-list) inverted-list))))
  (invert-internal char-list '()))

; read a file in and create a list of characters in inverse order
(define (cat port)
  (define (cat-internal port buffer)
    (if (eof-object? (peek-char port))
	buffer
	(cat-internal port 
		      (cons (read-char port) buffer))))

  (if (input-port? port) 
      (invert (cat-internal port '()))
      #f))


(cat in-port)
(cat "not a port")
      
(close-input-port in-port)

; testing read

(set! in-port (open-input-file "test/test_load.scm"))

(eval (read in-port))
(eval (read in-port))
(eval (read in-port))
(eval (read in-port))

(eof-object? (read in-port))

(close-input-port in-port)

(define out-port (open-output-file "garbage.txt"))

(write '(1 2 3 4 5.1 ) out-port)

(close-output-port out-port)

