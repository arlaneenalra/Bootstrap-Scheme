(define (id x) (dump_env) x)

(dump_env)

(id 1)

(dump_env)

(define (id2 x) (dump_env) (id x))

(id2 2)

(dump_env)

(quit)
