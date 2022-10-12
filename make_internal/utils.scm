(define define-library (lambda (name)
    (gmk-eval (string-append "libraries += " name))
    (gmk-eval (string-append name"_source_dirs = libs/" name))
    (gmk-eval (string-append name"_name = " name))
))

(define enable-library-tests (lambda (name)
    ; Ignore the tests folder in the library
    (gmk-eval (string-append name"_ignore = libs/" name "/tests*"))

    ; Just add the tests library
    (gmk-eval (string-append "libraries += " name "_tests"))
    (gmk-eval (string-append name"_tests_source_dirs = libs/" name "/tests"))
    (gmk-eval (string-append name"_tests_name = " name "_tests"))
    ; But the tests library must compile to a dynamic lib
    (gmk-eval (string-append name"_tests_target_type = dynlib"))
    ; And depends on the normal lib
    (gmk-eval (string-append name"_tests_depedencies += " name))
))

