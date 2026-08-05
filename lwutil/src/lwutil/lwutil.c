/**
 * \file            lwutil.c
 * \brief           Lightweight utility library
 */

/*
 * Copyright (c) 2024 Tilen MAJERLE
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
 * Version:         v1.4.0
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
    } while (--ptr_len > 0 && (byt & 0x80U) > 0);

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

/**
 * \brief           Check the the time between time now and time variable
 *                  is greater than the defined time_period.
 * 
 * When it is, the time_variable is updated accordingly, following the `2` possible scenarios:
 *  - Delta time is less than 2x period, the time variable is increased by period, to keep the drift-free updates
 *  - Delta time is more than 2x period, the time variable is set to current time, to resync the drift
 * 
 * The check if: `(time_now - *time_variable) >= time_period`
 * 
 * The function can be used as follows:
 * 
 * ```c
 * //At the top somewhere
 * static uint32_t time_last;
 * 
 * //Later periodically in the function calls
 * if (lwutil_tutil_has_elapsed(time_now, &time_last, 500)) {
 *  // Do something if time_now - time_last is at least 500 units apart
 *  // will also update the time_last to either +500, or to the time_now, depending on the delta
 * }
 * ```
 * 
 * \note            This is the 32-bit time variant
 * 
 * \param           time_now: Current time. It can be in ms or any other unit, as long as all variables hold same unit
 * \param           time_variable: Time variable to check against. It is a pointer to the variable
 * \param           time_period: Time period to check against
 * \return          `1` is time elapsed, `0` otherwise  
 */
uint8_t
lwutil_tutil_has_elapsed(const uint32_t time_now, uint32_t* const time_variable, const uint32_t time_period) {
    uint8_t retval = 0;

    if (time_variable != NULL) {
        const uint32_t delta = time_now - *time_variable;
        if (delta >= time_period) {
            retval = 1;

            if (delta >= 2 * time_period) {
                *time_variable = time_now;
            } else {
                *time_variable += time_period;
            }
        }
    }

    return retval;
}
