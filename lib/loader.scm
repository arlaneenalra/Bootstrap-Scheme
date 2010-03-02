;; Code used to load r6rs libraries during the boostrap proces

(provide import)


;; list of registered libraries
(define libs '())

;; load library definitions
(define (load-libs)
  (define fin
    (open-input-port "lib/libs.lib"))

  (set! libs (read fin))
  (close-input-port fin))

;; load an parse a library file
(define (read-library-inner file-name)
  (define fin 
    (open-input-port file-name))
  
  (define library
    (read fin))
  
  (close-input-port fin)
  library)

;; did we actually find a libray to load?
(define (read-library file-name)
  (if (string? file-name)
      (read-library-inner file-name)
      '()))

;; return only the name portion of a library reference 
(define (name-version library-ref)

  (define (inner ref name)
    (cond 

     ;; no version specified
     ((null? ref)
      (cons (reverse name) '()))

     ;; skip library escape
     ((equal? 'library (car ref))
      (inner (cadr ref) name))

     ((symbol? (car ref))
      (inner (cdr ref) (cons (car ref) name)))

     (else
      (cons (reverse name) (car ref)))))
     

  (inner library-ref '()))
  
;; does the given library match the given spec?
(define (library-match? library-ref library-name)
  (define ref 
    (name-version library-ref))

  (define lib
    (name-version library-name))

  ;; are the names equal?
  (equal? (car ref) (car lib)))
  


;; attempts to find a library with the 
;; given name in our list of reigstered
;; libraries
(define (get-library library-name)

  (define (inner library-name lib-list)
    (cond  
     
     ;; there is no matching library
     ((null? lib-list) '())

     ;; did we find the library?
     ((library-match? (caar lib-list) library-name) 
      (cdar lib-list))

     ;; check next record
     (else
	(inner library-name (cdr lib-list)))))

  (inner library-name libs))



;; import a library into the current execution environment
(define (import . import-list)
  
  ;; load the libs list if we don't 
  ;; have one defined already
  (for-each 
   (lambda (library)
     (define lib-def
       (read-library
	(get-library library)))
     
     (display "name:")
     (write (car (name-version lib-def)))
     (newline)
     
     (display "body:")
     (write (cddddr lib-def))
     (newline)
     )

   import-list)

  #t)

;; load library definitions
(load-libs)
