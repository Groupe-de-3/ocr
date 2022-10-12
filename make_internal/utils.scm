(define define-library (lambda (name . options)
    (define has_source_dirs #f)

    (for-each (lambda (option)
        (if (eq? (car option) 'source-dirs) (begin
            (gmk-eval (string-append name"_source_dirs += " (string-join (cdr option) " ")))
            (set! has_source_dirs #t)
        ))
        (if (eq? (car option) 'deps) (begin
            (gmk-eval (string-append name"_depedencies += " (string-join (cdr option) " ")))
        ))
        (if (eq? (car option) 'target-type) (begin
            (gmk-eval (string-append name"_target_type := " (string-join (cdr option) " ")))
        ))
        (if (eq? (car option) 'cflags) (begin
            (gmk-eval (string-append name"_cflags += " (string-join (cdr option) " ")))
        ))
        (if (eq? (car option) 'ignore) (begin
            (gmk-eval (string-append name"_ignore := " (string-join (cdr option) " ")))
        ))
    ) options)

    (gmk-eval (string-append "libraries += " name))
    (if (not has_source_dirs)
        (gmk-eval (string-append name"_source_dirs = libs/" name)))
    (gmk-eval (string-append name"_name = " name))
))

(define enable-library-tests (lambda (name)
    ; Ignore the tests folder in the library
    (gmk-eval (string-append name"_ignore = libs/" name "/tests*"))

    (define-library (string-append name"_tests")
        `(source-dirs ,(string-append "libs/" name "/tests"))
        `(target-type "dynlib")
        `(deps ,name)
    )
))

