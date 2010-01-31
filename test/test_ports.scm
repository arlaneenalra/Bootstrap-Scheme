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
  (invert (cat-internal port '())))


(cat in-port)
      

(close-input-port in-port)


