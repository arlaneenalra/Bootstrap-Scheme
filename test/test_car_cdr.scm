

(load "lib/stdlib.scm")

;;(list-list->list-string car-cdr-list)

(car-cdr-gen-proc (car car-cdr-list))
(car-cdr-gen-proc (car (cdr car-cdr-list)))