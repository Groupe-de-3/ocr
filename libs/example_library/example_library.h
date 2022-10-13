#pragma once

/*! \file example_library.h
 *
 *  \brief This header file contains every function and type definition of the
 * example library.
 *
 *  This library is only an example for showing how the build system and other
 * works. It only contains the useless #add function.
 */

/*! \brief Adds the two given numbers
 *
 *  Uses the standard c '+' operator to add the given two numbers together.
 *
 *  \param a The first number
 *  \param b The second number
 *  \return The result of the addition
 */
int add(int a, int b);

/*! \brief Substract the two given numbers.
 *
 *  Uses the standard c '-' operator to substract b from a.
 *
 *  \param a The first number
 *  \param b The second number
 *  \return The result of the substraction of b from a.
 */
int sub(int a, int b);
