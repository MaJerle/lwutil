#include "lwutil/lwutil.h"
#include "../test.h"

int
test_run(void) {
    int retval = 0;

    lwutil_rregslope_t rrs;
    int32_t buf4[4], buf8[8], buf16[16];
    int32_t slope;

    /* Init argument validation: NULL rrs/buffer and too-small buffer_len must be rejected */
    TEST_IF_TRUE(lwutil_rregslope_init(NULL, buf4, 4) == 0);
    TEST_IF_TRUE(lwutil_rregslope_init(&rrs, NULL, 4) == 0);
    TEST_IF_TRUE(lwutil_rregslope_init(&rrs, buf4, 0) == 0);
    TEST_IF_TRUE(lwutil_rregslope_init(&rrs, buf4, 1) == 0);
    TEST_IF_TRUE(lwutil_rregslope_init(&rrs, buf4, 2) == 1);

    /* add_value/compute_slope with NULL instance must not crash and must report failure */
    TEST_IF_TRUE(lwutil_rregslope_add_value(NULL, 123) == 0);
    TEST_IF_TRUE(lwutil_rregslope_compute_slope(NULL, &slope) == 0);

    /* get_capacity/get_count with NULL instance must not crash and must report 0 */
    TEST_IF_TRUE(lwutil_rregslope_get_capacity(NULL) == 0);
    TEST_IF_TRUE(lwutil_rregslope_get_count(NULL) == 0);

    /*
     * For an exact arithmetic progression y = base + step * i, the least-squares slope
     * of any window taken from it is exactly `step`, regardless of window size or base.
     * This lets the expected result be known exactly, without floating point.
     */
    typedef struct {
        int32_t* buffer;
        size_t capacity;
    } rrs_buf_t;

    const rrs_buf_t bufs[] = {
        {buf4, LWUTIL_ARRAYSIZE(buf4)},
        {buf8, LWUTIL_ARRAYSIZE(buf8)},
        {buf16, LWUTIL_ARRAYSIZE(buf16)},
    };

    for (size_t b = 0; b < LWUTIL_ARRAYSIZE(bufs); ++b) {
        int32_t* const buffer = bufs[b].buffer;
        const size_t capacity = bufs[b].capacity;

        TEST_IF_TRUE(lwutil_rregslope_init(&rrs, buffer, capacity) == 1);

        /* get_capacity reflects the buffer_len passed to init, and never changes afterwards */
        TEST_IF_TRUE(lwutil_rregslope_get_capacity(&rrs) == capacity);
        TEST_IF_TRUE(lwutil_rregslope_get_count(&rrs) == 0);

        /* Slope must not be available (buffer_full flag == 0) until the window is completely full */
        for (size_t i = 0; i < capacity; ++i) {
            slope = 0xAAAAAAAA;
            TEST_IF_TRUE(lwutil_rregslope_add_value(&rrs, (int32_t)(10 + 3 * i)) == 1);
            /* get_count tracks the number of stored samples, capped at capacity */
            TEST_IF_TRUE(lwutil_rregslope_get_count(&rrs) == i + 1);
            TEST_IF_TRUE(lwutil_rregslope_get_capacity(&rrs) == capacity);
            if (i + 1 < capacity) {
                TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 0);
                TEST_IF_TRUE(slope == (int32_t)0xAAAAAAAA); /* Must be left untouched */
            } else {
                TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 1);
                TEST_IF_TRUE(slope == 3);
            }
        }
        /* compute_slope(..., NULL) must only report availability, without touching any output */
        TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, NULL) == 1);

        /* Completely replace the window (exactly `capacity` new samples) with a steeper positive slope */
        for (size_t i = 0; i < capacity; ++i) {
            TEST_IF_TRUE(lwutil_rregslope_add_value(&rrs, (int32_t)(1000 + 7 * i)) == 1);
            /* Window is already full, so count must stay saturated at capacity while sliding */
            TEST_IF_TRUE(lwutil_rregslope_get_count(&rrs) == capacity);
            TEST_IF_TRUE(lwutil_rregslope_get_capacity(&rrs) == capacity);
        }
        TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 1);
        TEST_IF_TRUE(slope == 7);

        /* Completely replace the window with a negative slope */
        for (size_t i = 0; i < capacity; ++i) {
            TEST_IF_TRUE(lwutil_rregslope_add_value(&rrs, (int32_t)(500 - 5 * (int32_t)i)) == 1);
        }
        TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 1);
        TEST_IF_TRUE(slope == -5);

        /* Completely replace the window with constant values -> slope of 0 */
        for (size_t i = 0; i < capacity; ++i) {
            TEST_IF_TRUE(lwutil_rregslope_add_value(&rrs, 42) == 1);
        }
        TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 1);
        TEST_IF_TRUE(slope == 0);
    }
    {
        /*
         * Run the test for longer time to actually slide the window
         * and check if the old value goes out properly
         */
        const int32_t step = 4;
        const int32_t base = -100;
        int32_t buffer[4];

        TEST_IF_TRUE(lwutil_rregslope_init(&rrs, buffer, LWUTIL_ASZ(buffer)) == 1);
        for (size_t i = 0; i < 2 * LWUTIL_ASZ(buffer); ++i) {
            TEST_IF_TRUE(lwutil_rregslope_add_value(&rrs, (int32_t)(base + step * (int32_t)i)) == 1);
            if (i + 1 < LWUTIL_ASZ(buffer)) {
                TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 0);
            } else {
                TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 1);
                TEST_IF_TRUE(slope == step);
            }
        }
    }

    /* reset(NULL) must not crash and must report failure */
    TEST_IF_TRUE(lwutil_rregslope_reset(NULL) == 0);

    {
        /*
         * The window-size-dependent constants (capacity, buffer pointer, sum_x, denom)
         * are computed once by init() and must never be touched by reset(). Only the
         * rolling state (index, count, sum_Y, sum_xY) is expected to go back to zero.
         */
        int32_t buffer[6];

        TEST_IF_TRUE(lwutil_rregslope_init(&rrs, buffer, LWUTIL_ASZ(buffer)) == 1);

        /* Fully fill the window with non-trivial values so all rolling sums become non-zero */
        for (size_t i = 0; i < LWUTIL_ASZ(buffer); ++i) {
            TEST_IF_TRUE(lwutil_rregslope_add_value(&rrs, (int32_t)(17 + 5 * i)) == 1);
        }
        TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 1);
        TEST_IF_TRUE(slope == 5);

        /* Snapshot the constant parameters before reset */
        const size_t capacity_before = rrs.capacity;
        int32_t* const buffer_ptr_before = rrs.buffer;
        const int64_t sum_x_before = rrs.sum_x;
        const int64_t denom_before = rrs.denom;

        /* Sanity check: rolling state is indeed non-zero prior to reset */
        TEST_IF_TRUE(rrs.count != 0);
        TEST_IF_TRUE(rrs.sum_Y != 0);
        TEST_IF_TRUE(rrs.sum_xY != 0);

        /* get_count/get_capacity must agree with the raw struct fields before reset too */
        TEST_IF_TRUE(lwutil_rregslope_get_count(&rrs) == LWUTIL_ASZ(buffer));
        TEST_IF_TRUE(lwutil_rregslope_get_capacity(&rrs) == LWUTIL_ASZ(buffer));

        TEST_IF_TRUE(lwutil_rregslope_reset(&rrs) == 1);

        /* Rolling state must be cleared to zero */
        TEST_IF_TRUE(rrs.index == 0);
        TEST_IF_TRUE(rrs.count == 0);
        TEST_IF_TRUE(rrs.sum_Y == 0);
        TEST_IF_TRUE(rrs.sum_xY == 0);

        /* Constant parameters must be left completely untouched */
        TEST_IF_TRUE(rrs.capacity == capacity_before);
        TEST_IF_TRUE(rrs.buffer == buffer_ptr_before);
        TEST_IF_TRUE(rrs.sum_x == sum_x_before);
        TEST_IF_TRUE(rrs.denom == denom_before);

        /* get_count must drop back to 0, while get_capacity must stay exactly as before reset */
        TEST_IF_TRUE(lwutil_rregslope_get_count(&rrs) == 0);
        TEST_IF_TRUE(lwutil_rregslope_get_capacity(&rrs) == capacity_before);

        /* Slope must be unavailable right after reset, since the window is no longer full */
        TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 0);

        /* After reset, the instance must behave exactly like a freshly-initialized one:
           refilling the (still valid) window must produce a correct slope again */
        for (size_t i = 0; i < LWUTIL_ASZ(buffer); ++i) {
            slope = 0xAAAAAAAA;
            TEST_IF_TRUE(lwutil_rregslope_add_value(&rrs, (int32_t)(-30 + 2 * (int32_t)i)) == 1);
            TEST_IF_TRUE(lwutil_rregslope_get_count(&rrs) == i + 1);
            TEST_IF_TRUE(lwutil_rregslope_get_capacity(&rrs) == LWUTIL_ASZ(buffer));
            if (i + 1 < LWUTIL_ASZ(buffer)) {
                TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 0);
                TEST_IF_TRUE(slope == (int32_t)0xAAAAAAAA);
            } else {
                TEST_IF_TRUE(lwutil_rregslope_compute_slope(&rrs, &slope) == 1);
                TEST_IF_TRUE(slope == 2);
            }
        }

        /* Resetting a freshly-initialized (never-used) instance must keep it all-zero
           and must not disturb its constant parameters either */
        int32_t buffer2[5];
        TEST_IF_TRUE(lwutil_rregslope_init(&rrs, buffer2, LWUTIL_ASZ(buffer2)) == 1);

        const size_t capacity_before2 = rrs.capacity;
        int32_t* const buffer_ptr_before2 = rrs.buffer;
        const int64_t sum_x_before2 = rrs.sum_x;
        const int64_t denom_before2 = rrs.denom;

        TEST_IF_TRUE(lwutil_rregslope_get_capacity(&rrs) == LWUTIL_ASZ(buffer2));
        TEST_IF_TRUE(lwutil_rregslope_get_count(&rrs) == 0);

        TEST_IF_TRUE(lwutil_rregslope_reset(&rrs) == 1);

        TEST_IF_TRUE(rrs.index == 0);
        TEST_IF_TRUE(rrs.count == 0);
        TEST_IF_TRUE(rrs.sum_Y == 0);
        TEST_IF_TRUE(rrs.sum_xY == 0);

        TEST_IF_TRUE(rrs.capacity == capacity_before2);
        TEST_IF_TRUE(rrs.buffer == buffer_ptr_before2);
        TEST_IF_TRUE(rrs.sum_x == sum_x_before2);
        TEST_IF_TRUE(rrs.denom == denom_before2);

        TEST_IF_TRUE(lwutil_rregslope_get_capacity(&rrs) == capacity_before2);
        TEST_IF_TRUE(lwutil_rregslope_get_count(&rrs) == 0);
    }
    return retval;
}
