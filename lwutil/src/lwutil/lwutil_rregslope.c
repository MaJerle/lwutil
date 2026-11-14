/**
 * \file            lwutil_rregslope.c
 * \brief           Rolling regression slope
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
 * \brief           Initialize the rolling regression slope instance
 *
 * \param           rrs: Instance to initialize
 * \param           buffer: External buffer used to store the rolling window of values.
 *                      Must remain valid for the lifetime of `rrs`
 * \param           buffer_len: Number of elements in `buffer`, the window size.
 *                      Must be at least `2`, since a slope needs at least 2 points
 * \return          `1` on success, `0` on failure
 */
uint8_t
lwutil_rregslope_init(lwutil_rregslope_t* rrs, int32_t* buffer, size_t buffer_len) {
    uint8_t ret = 0;

    if (rrs != NULL && buffer != NULL && buffer_len >= 2) {
        const int64_t n = (int64_t)buffer_len;

        /* Sum of all Xes and sum of all X^2 in a sequence */
        const int64_t sum_x = (n * (n - 1)) / 2;
        const int64_t sum_x2 = ((n - 1) * n * (2 * n - 1)) / 6;

        lwutil_rregslope_reset(rrs);

        rrs->capacity = buffer_len;
        rrs->buffer = buffer;

        /* Constants of the least-squares formula for a full window,
            only depend on the window size */
        rrs->sum_x = sum_x;
        rrs->denom = (n * sum_x2) - (sum_x * sum_x);

        ret = 1;
    }
    return ret;
}

/**
 * \brief           Add a new value to the rolling window
 *
 * \param           rrs: Instance to work with
 * \param           value: New value to add
 * \return          `1` on success
 */
uint8_t
lwutil_rregslope_add_value(lwutil_rregslope_t* rrs, int32_t value) {
    if (rrs == NULL) {
        return 0;
    }
    if (rrs->count < rrs->capacity) {
        /* Window not yet full: append at the next free slot, x = count (0-based sample position) */
        rrs->buffer[rrs->index] = value;
        rrs->sum_Y += value;
        rrs->sum_xY += (int64_t)value * (int64_t)rrs->count;

        ++rrs->count;
    } else {
        /* Window full: drop the oldest sample and slide the window by one, see comment above for the derivation */
        const int32_t oldest_value = rrs->buffer[rrs->index];
        const int32_t old_sum_Y = rrs->sum_Y;
        const int64_t old_sum_xY = rrs->sum_xY;

        /* Every remaining sample's weight (x) drops by 1; the new sample becomes the newest, at x = capacity - 1 */
        rrs->buffer[rrs->index] = value;
        rrs->sum_xY = old_sum_xY - ((int64_t)old_sum_Y - (int64_t)oldest_value) + (int64_t)(rrs->capacity - 1) * value;
        rrs->sum_Y = old_sum_Y - oldest_value + value;
    }

    if (++rrs->index >= rrs->capacity) {
        rrs->index = 0;
    }
    return 1;
}

/**
 * \brief           Compute the least-squares slope of the current window
 *
 * The window must be full before a slope is meaningful, since the precomputed
 * `sum_x` / `denom` constants assume `capacity` samples.
 *
 * \param           rrs: Instance to work with
 * \param[out]      slope: Pointer to store the computed slope.
 *                      Can be set to `NULL` to only check if the slope is available
 * \return          `1` if the window is full and `slope` was computed, `0` otherwise
 */
uint8_t
lwutil_rregslope_compute_slope(const lwutil_rregslope_t* rrs, int32_t* slope) {
    uint8_t ret = 0;

    if (rrs != NULL && rrs->count == rrs->capacity) {
        const int64_t numerator = (int64_t)rrs->capacity * rrs->sum_xY - rrs->sum_x * (int64_t)rrs->sum_Y;

        if (slope != NULL) {
            *slope = (int32_t)(numerator / rrs->denom);
        }
        ret = 1;
    }
    return ret;
}

/**
 * \brief           Reset the rolling regression window,
 *                  clear the history and all rolling sums to zero
 *
 * \param           rrs: Rolling regression instance to work with
 * \return          `1` on success, `0` otherwise
 */
uint8_t
lwutil_rregslope_reset(lwutil_rregslope_t* rrs) {
    uint8_t ret = 0;

    if (rrs != NULL) {
        rrs->index = 0;
        rrs->count = 0;
        rrs->sum_Y = 0;
        rrs->sum_xY = 0;

        ret = 1;
    }
    return ret;
}
