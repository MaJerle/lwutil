/**
 * \file            lwutil_varint.c
 * \brief           Variable length integer utilities
 */

/*
 * Copyright (c) 2026 Tilen MAJERLE
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
 * Version:         v1.6.1
 */
#include <stddef.h>
#include <stdint.h>
#include "lwutil/lwutil.h"

/**
 * \brief           Load variable length integer from the byte stream to the variable.
 *
 * Variable length integers (short varints) are stored with continuation bit (bit 7, 0x80),
 * which tells us if there is a next byte to be used as part of the number sequence.
 *
 * 32-bit integer can be stored with anything between `1` and `5` bytes.
 *
 * \param           ptr: Array pointer to load data from
 * \param           ptr_len: Input array length
 * \param           val_out: Pointer to variable to write result value
 * \return          Number of bytes used to load the full number. Set to `0` in case of an error.
 */
uint8_t
lwutil_ld_u32_varint(const void* ptr, size_t ptr_len, uint32_t* val_out) {
    const size_t max_bytes = 5;
    size_t cnt = 0;
    uint32_t val = 0;
    const uint8_t* p_data = ptr;
    uint8_t byt;

    if (ptr == NULL || ptr_len == 0 || val_out == NULL) {
        return 0;
    }
    do {
        byt = *p_data++;
        val |= ((uint32_t)(byt & 0x7FU)) << (cnt * 7U);
        ++cnt;
    } while (--ptr_len > 0 && cnt < max_bytes && (byt & 0x80U) > 0);

    /* Check memory length */
    if ((byt & 0x80U) > 0) {
        val = 0;
        cnt = 0;
    }
    *val_out = val;
    return cnt;
}

/**
 * \brief           Store an integer into variable length byte sequence array.
 *
 * Variable length integers (short varints) are stored with continuation bit (bit 7, 0x80),
 * which tells us if there is a next byte to be used as part of the number sequence.
 *
 * 32-bit integer can be stored with anything between `1` and `5` bytes.
 *
 * \param           val: Value to encode into byte sequence
 * \param           ptr: Array to write output result
 * \param           ptr_len: Length of an input array
 * \return          Number of bytes written (stored) in the memory. Set to `0` in case of an error.
 */
uint8_t
lwutil_st_u32_varint(uint32_t val, void* ptr, size_t ptr_len) {
    uint8_t* p_data = ptr;
    size_t cnt = 0;

    if (ptr == NULL || ptr_len == 0) {
        return 0;
    }
    do {
        *p_data++ = (val & 0x7FU) | (val > 0x7FU ? 0x80U : 0x00U);
        val >>= 7U;
        ++cnt;
    } while (--ptr_len > 0 && val > 0);

    /* Memory check */
    if (val > 0) {
        cnt = 0;
    }
    return cnt;
}
