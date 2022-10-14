(define define-target (lambda (name . options)
    ; Set to true of the option source-dirs is present at least once
    (define has_source_dirs #f)
    (define has_enable_tests #f)
    (define target_type "staticlib")
    (define link_in_deps 'default)

    (for-each (lambda (option) (cond
        ((eq? (car option) 'enable-tests)
            (if has_enable_tests
                (error "Cannot enable tests multiple time")
                (begin
                    (set! has_enable_tests #t)
                    (enable-target-tests name)
                ))
        )
        ((eq? (car option) 'link-in-deps)
            (if (and (> (length option) 1) (equal? (cadr option) #f))
                (set! link_in_deps #f)
                (set! link_in_deps #t)
            )
        )
        ((eq? (car option) 'source-dirs) (begin
            (gmk-eval (string-append name"_source_dirs += " (string-join (cdr option) " ")))
            (set! has_source_dirs #t)
        ))
        ((eq? (car option) 'deps)
            (gmk-eval (string-append name"_depedencies += " (string-join (cdr option) " ")))
        )
        ((eq? (car option) 'cflags)
            (gmk-eval (string-append name"_cflags += " (string-join (cdr option) " ")))
        )
        ((eq? (car option) 'target-type)
            (set! target_type (cadr option))
        )
        ((eq? (car option) 'ignore)
            (gmk-eval (string-append name"_ignore := " (string-join (cdr option) " ")))
        )
        ((eq? (car option) 'run-args)
            (gmk-eval (string-append name"_run_arguments += " (string-join (cdr option) " ")))
        )
        (else
            (error "Invalid target option" (symbol->string (car option)))
        )
    )) options)
        
    ; The default value for link_in_deps
    ; is only false for static libs, true in other cases.
    (if (eq? link_in_deps 'default)
        (set! link_in_deps (not (equal? target_type "staticlib"))))

    (gmk-eval (string-append name"_target_type := "target_type))
    (gmk-eval (string-append "targets += "name))

    (if link_in_deps
        (gmk-eval (string-append name"_link_in_deps = true")))

    (if (not has_source_dirs)
        (cond
            ((equal? target_type "executable")
                (gmk-eval (string-append name"_source_dirs = executables/"name)))
            (else
                (gmk-eval (string-append name"_source_dirs = libs/"name)))
    ))
    (gmk-eval (string-append name"_name = " name))
))

(define enable-target-tests (lambda (name)
    ; Ignore the tests folder in the library
    (gmk-eval (string-append name"_ignore = $(addsuffix /tests*,$("name"_source_dirs))"))

    (define-target (string-append "__"name"_tests")
        `(source-dirs ,(string-append "$(addsuffix /tests,$("name"_source_dirs))"))
        `(target-type "dynlib")
        `(deps ,name "test-lib")
        `(link-in-deps #t)
    )
    (gmk-eval (string-append "test_targets += __"name"_tests"))
))

