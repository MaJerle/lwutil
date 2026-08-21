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

        /* Slope must not be available (buffer_full flag == 0) until the window is completely full */
        for (size_t i = 0; i < capacity; ++i) {
            slope = 0xAAAAAAAA;
            TEST_IF_TRUE(lwutil_rregslope_add_value(&rrs, (int32_t)(10 + 3 * i)) == 1);
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
    return retval;
}
