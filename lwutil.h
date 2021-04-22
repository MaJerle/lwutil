/**
 * \file            lwutil.h
 * \brief           Lightweight utility library
 */

/*
 * Copyright (c) 2020 Tilen MAJERLE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of LwUTIL - Lightweight utility library.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         $_version_$
 */
#ifndef LWUTIL_HDR_H
#define LWUTIL_HDR_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * \defgroup        LWUTIL Lightweight utility
 * \brief           Lightweight utility
 * \{
 */

/**
 * \brief           Get size of statically allocated array
 * Array must be declared in a form of `type var_name[element_count]`

 * \param[in]       x: Object to get array size of
 * \return          Number of elements in array (`element_count`)
 */
#define LWUTIL_ARRAYSIZE(x)                     (sizeof(x) / sizeof((x)[0]))

/**
 * \brief           Get larger value out of 2 different inputs
 * \param[in]       x: First input
 * \param[in]       y: Second input
 * \return          Larger of both inputs
 */
#define LWUTIL_MAX(x, y)                        ((x) > (y) ? (x) : (y))

/**
 * \brief           Get smaller value out of 2 different inputs
 * \param[in]       x: First input
 * \param[in]       y: Second input
 * \return          Smaller of both inputs
 */
#define LWUTIL_MIN(x, y)                        ((x) < (y) ? (x) : (y))

/**
 * \brief           Get absolute value of the input
 * Returns always-positive value of the input
 * \param[in]       x: Input value
 * \return          Absolute value of the input value
 */
#define LWUTIL_ABS(x)                           ((x) < 0 ? -(x) : (x))

/**
 * \brief           Unused variable to avoid compilation warning if declared but not used
 * \param[in]       x: Input variable to declare unused
 */
#define LWUTIL_UNUSED(x)                        (void)(a)

#define LWUTIL_CONCAT_BASE(x, y)                x ## y
#define LWUTIL_CONCAT(s0, s1)                   LWC_CONCAT_BASE(s0, s1)

/**
 * \brief           Compile time assert to validate specific expression
 * Produces compilation error if expression is false
 * \param[in]       exp: Expression to test
 * \param[in]       random_variable_name: Meaningful variable name to be used.
 *                      Can be whatever until it is valid variable name
 */
#define LWUTIL_COMPILE_TIME_ASSERT(exp, random_variable_name)\
    typedef char LWC_CONCAT2(random_variable_name, __LINE__)[!(exp) ? -1 : 1];

void        lwutil_st_u16_le(uint16_t val, void* ptr);
void        lwutil_st_u32_le(uint32_t val, void* ptr);
uint16_t    lwutil_ld_u16_le(const void* ptr);
uint32_t    lwutil_ld_u32_le(const void* ptr);

void        lwutil_st_u16_be(uint16_t val, void* ptr);
void        lwutil_st_u32_be(uint32_t val, void* ptr);
uint16_t    lwutil_ld_u16_be(const void* ptr);
uint32_t    lwutil_ld_u32_be(const void* ptr);

void        lwutil_u8_to_2asciis(uint8_t hex, char* ascii);
void        lwutil_u16_to_4asciis(uint16_t hex, char* ascii);
void        lwutil_u32_to_8asciis(uint32_t hex, char* ascii);

/**
 * \}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LWUTIL_HDR_H */

