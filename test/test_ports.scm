; This must be the first line

; open the port
(define in-port (open-input-file "test/test_ports.scm"))

; read a file in and create a list of characters in inverse order
(define (invert port)
  (define (invert-internal port buffer)
    (if (eof-object? (peek-char port))
	buffer
	(invert-internal port 
		      (cons (read-char port) buffer))))
  (invert-internal port '()))


(invert in-port)
      

(close-input-port in-port)


