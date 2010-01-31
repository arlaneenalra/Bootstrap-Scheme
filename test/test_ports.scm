; This must be the first line

; open the port
(define in-port (open-input-file "test/test_ports.scm"))

(read-char in-port)
(peek-char in-port)
(read-char in-port)
(read-char in-port)
(read-char in-port)

(close-input-file in-port)


