#pragma once

/*! \file utils_countargs.h
 *
 *  This file only contains the definition of the #utils_countargs macro.
 *  It is part of the "utils" library.
 */

/*! \brief Helper used by #utils_countargs. Do not use directly
 *
 */
#define utils_countargs_helper(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N

/*! \brief Expands to the number of arguments given to it, up to 9.
 *
 *  Can be used to implicitly find the size of an array.
 *  No arguments gets evaluated.
 *
 *  \par Example
 *  \parblock \code
 *  utils_countargs(a, b, 1, 12, 5 + t.T) // Expands to 5
 *  \endcode \endparblock
 */
#define utils_countargs(...) \
    utils_countargs_helper(__VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
