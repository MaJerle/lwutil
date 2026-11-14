/**
 * \file            lwutil.c
 * \brief           Lightweight utility library
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
#include <stdlib.h>
#include <string.h>
#include "lwutil/lwutil.h"

/**
 * \brief           Makes ascii char array from `unsigned 8-bit` value
 * \param[in]       hex: Hexadecimal data to be converted
 * \param[out]      ascii: Minimum `3-bytes` long array to write value to
 */
void
lwutil_u8_to_2asciis(uint8_t hex, char* ascii) {
    for (uint8_t idx = 2U; idx != 0; --idx) {
        uint8_t aux = (uint8_t)(hex >> (4U * (idx - 1U))) & 0x0FU;

        aux = (aux <= 9U) ? (aux + 0x30U) : (aux + 0x57U);
        ascii[2U - idx] = (char)aux;
    }
    ascii[2U] = '\0';
}

/**
 * \brief           Makes ascii char array from `unsigned 16-bit` value
 * \param[in]       hex: Hexadecimal data to be converted
 * \param[out]      ascii: Minimum `5-bytes` long array to write value to
 */
void
lwutil_u16_to_4asciis(uint16_t hex, char* ascii) {
    for (uint8_t idx = 4U; idx != 0; --idx) {
        uint8_t aux = (uint8_t)(hex >> (4U * (idx - 1U))) & 0x0FU;

        aux = (aux <= 9U) ? (aux + 0x30U) : (aux + 0x57U);
        ascii[4U - idx] = (char)aux;
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
    for (uint8_t idx = 8U; idx != 0; --idx) {
        uint8_t aux = (uint8_t)(hex >> (4U * (idx - 1U))) & 0x0FU;

        aux = (aux <= 9U) ? (aux + 0x30U) : (aux + 0x57U);
        ascii[8U - idx] = (char)aux;
    }
    ascii[8] = '\0';
}
