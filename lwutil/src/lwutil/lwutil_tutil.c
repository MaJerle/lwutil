/**
 * \file            lwutil_tutil.c
 * \brief           Time utilities
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

            /*
             * When the delta is much larger than the period,
             * we set the time to the new time, and not to the time_period aligned time_variable
             *
             * This is to simplify the process or else we would enter into the
             * remainder arithmetic which might be slow for small CPUs
             */
            if (delta >= (2 * time_period)) {
                *time_variable = time_now;
            } else {
                *time_variable += time_period;
            }
        }
    }

    return retval;
}
