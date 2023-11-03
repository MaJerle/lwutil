/**
 * \file            lwutil.c
 * \brief           Lightweight utility library
 */

/*
 * Copyright (c) 2023 Tilen MAJERLE
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
 * Version:         v1.0.0
 */
#include <string.h>
#include "lwutil/lwutil.h"

/**
 * \brief           Makes ascii char array from `unsigned 8-bit` value
 * \param[in]       hex: Hexadecimal data to be converted
 * \param[out]      ascii: Minimum `3-bytes` long array to write value to
 */
void
lwutil_u8_to_2asciis(uint8_t hex, char* ascii) {
    for (uint8_t i = 2; i != 0; --i) {
        uint8_t aux = (hex >> (4 * (i - 1))) & 0x0F;

        aux = (aux <= 9) ? (aux + 0x30) : (aux + 0x57);
        ascii[2 - i] = (char)aux;
    }
    ascii[2] = '\0';
}

/**
 * \brief           Makes ascii char array from `unsigned 16-bit` value
 * \param[in]       hex: Hexadecimal data to be converted
 * \param[out]      ascii: Minimum `5-bytes` long array to write value to
 */
void
lwutil_u16_to_4asciis(uint16_t hex, char* ascii) {
    for (uint8_t i = 4; i != 0; --i) {
        uint8_t aux = (hex >> (4 * (i - 1))) & 0x0F;

        aux = (aux <= 9) ? (aux + 0x30) : (aux + 0x57);
        ascii[4 - i] = (char)aux;
    }
    ascii[4] = '\0';
}

/**
 * \brief           Makes ascii char array from `unsigned 32-bit` value
 * \param[in]       hex: Hexadecimal data to be converted
 * \param[out]      ascii: Minimum `9-bytes` long array to write value to
 */
void
lwutil_u32_to_8asciis(uint32_t hex, char* ascii) {
    for (uint8_t i = 8; i != 0; --i) {
        uint8_t aux = (hex >> (4 * (i - 1))) & 0x0F;

        aux = (aux <= 9) ? (aux + 0x30) : (aux + 0x57);
        ascii[8 - i] = (char)aux;
    }
    ascii[8] = '\0';
}

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
 * \return          Number of bytes written (stored). `0` in case of an error.
 */
uint8_t
lwutil_ld_u32_varint(const void* ptr, size_t ptr_len, uint32_t* val_out) {
    size_t cnt = 0;
    uint32_t val = 0;
    const uint8_t* p = ptr;
    uint8_t b;

    if (ptr == NULL || ptr_len == 0 || val_out == NULL) {
        return 0;
    }
    do {
        b = *p++;
        val |= ((uint32_t)(b & 0x7F)) << (cnt * 7);
        ++cnt;
    } while (--ptr_len > 0 && (b & 0x80) > 0);

    /* Check memory length */
    if ((b & 0x80) > 0) {
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
 * \return          Number of bytes written (stored). `0` in case of an error.
 */
uint8_t
lwutil_st_u32_varint(uint32_t val, void* ptr, size_t ptr_len) {
    uint8_t* p = ptr;
    size_t cnt = 0;
    if (ptr == NULL || ptr_len == 0) {
        return 0;
    }
    do {
        *p++ = (val & 0x7F) | (val > 0x7F ? 0x80 : 0x00);
        val >>= 7;
        ++cnt;
    } while (--ptr_len > 0 && val > 0);

    /* Memory check */
    if (val > 0) {
        cnt = 0;
    }
    return cnt;
}
