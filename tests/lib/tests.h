#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*! \file tests.h
 *  \brief Host of helper macros and functions for writing tests with the
 *  internal test library.
 *
 *  *Heavy* inspiration from the [check](https://github.com/libcheck/check)
 *  library's api.
 */

#define H_ATTRIBUTE_NORETURN __attribute__((noreturn))

void _h_assert_failed(
    const char *file, int line, const char *expr, const char *msg, ...
) H_ATTRIBUTE_NORETURN;

#define h_assert_msg(expr, ...)                                               \
    (expr)                                                                    \
        ? (void)0                                                             \
        : _h_assert_failed(                                                   \
              __FILE__, __LINE__, "Assertion '" #expr "' failed", __VA_ARGS__ \
          )

#define h_assert(expr) h_assert_msg(expr, NULL)

#define _h_assert_op(TYPE, FORMAT_FLAGS, X, OP, Y)        \
    do {                                                  \
        TYPE _h_x = (X);                                  \
        TYPE _h_y = (Y);                                  \
        h_assert_msg(                                     \
            _h_x OP _h_y,                                 \
            "Assertion '%s' failed: %s == %" FORMAT_FLAGS \
            ", %s == %" FORMAT_FLAGS,                     \
            #X " " #OP " " #Y, #X, _h_x, #Y, _h_y         \
        );                                                \
    } while (0)

#define h_assert_int(X, OP, Y) _h_assert_op(intmax_t, "jd", X, OP, Y)
#define h_assert_uint(X, OP, Y) _h_assert_op(uintmax_t, "ju", X, OP, Y)
