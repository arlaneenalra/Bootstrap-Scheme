(interaction-environment)

(define env (null-environment))

(interaction-environment)


(eval '(define x 2))

(eval '(define z 1) env)

(eval 'z env)
(eval 'x env)

z

x

(interaction-environment)

env

(quit)
