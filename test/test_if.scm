; Check truth
(if #t 1 0)
(if #t 0 1)
(if #t 1)

; Check false
(if #f 1 0)
(if #f 0 1)
(if #f 1)

; Check non-boolean
(if 1 1 0)
(if 1 0 1)
(if 1 1)

(quit)
