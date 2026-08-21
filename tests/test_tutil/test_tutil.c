#include <stdio.h>
#include "lwutil/lwutil.h"
#include "../test.h"

int
test_run(void) {
    int retval = 0;

    uint32_t time_now, time_last;
    uint8_t val;

    typedef struct {
        uint32_t time_now;
        uint32_t time_expected;
        uint32_t val_expected;
    } test_time_tutil_data_t;

    /* NULL time_variable must be rejected without crashing */
    val = lwutil_tutil_has_elapsed(1000U, NULL, 500U);
    TEST_IF_TRUE(val == 0);

    /* Restart the calls*/
    time_last = 0;
    time_now = 0;

    /* Initial stage, time not elapsed */
    val = lwutil_tutil_has_elapsed(time_now, &time_last, 500);
    TEST_IF_TRUE(val == 0);

    /* Set the time, expect the time variable to match */
    time_now = 500;
    time_last = 0;
    val = lwutil_tutil_has_elapsed(time_now, &time_last, 500);
    TEST_IF_TRUE(val == 1);
    TEST_IF_TRUE(time_last == 500);

    /*
     * Set the time above the target but not too much
     * We expect time variable to advance for the period
     */
    time_now = 600;
    time_last = 0;
    val = lwutil_tutil_has_elapsed(time_now, &time_last, 500);
    TEST_IF_TRUE(val == 1);
    TEST_IF_TRUE(time_last == 500);

    /*
     * Set the time above the target, much above (2* the target above),
     * we now expect time_last to match the time_now to resync back
     */
    time_now = 1100;
    time_last = 0;
    val = lwutil_tutil_has_elapsed(time_now, &time_last, 500);
    TEST_IF_TRUE(val == 1);
    TEST_IF_TRUE(time_last == 1100);

    /*
     * Test data assumes step is 500ms
     */
    const test_time_tutil_data_t data_entries[] = {
        //{.time_now = 0, .time_expected = 0, .val_expected = 0},
        {0U, 0U, 0U},       // Starting point
        {200, 0U, 0U},      // First 200ms
        {400, 0U, 0U},      // Another 400ms
        {600U, 500U, 1U},   // First elapse happens here
        {800U, 500U, 0U},   // No new elapse
        {1000U, 1000U, 1U}, // 1000 reached, elapse triggered
        {1750U, 1500U, 1U}, // Another elapsed reached
        {2750U, 2750U, 1U}, // Elapsed reached, but this time it is at least 2x the delta, so make it equal
        {3000U, 2750U, 0U}, // No elapse since last check
        {3250U, 3250U, 1U}, // Elapse happened

        /* Add here the cases for overflow situation */
        {0xFFFFFFF0U, 0xFFFFFFF0U, 1U}, //Elapse happened from the previous run, large gap, make it equal
        {0U, 0xFFFFFFF0U, 0U},          //Overflow happens here
        {500U, 484U, 1U},               // Elapse after 500
        {500U, 484U, 0U},               // No new elapse
    };

    /* Starting point goes here */
    time_now = 0UL;
    time_last = 0UL;
    val = 0U;
    for (size_t idx = 0; idx < (sizeof(data_entries) / sizeof(data_entries[0])); ++idx) {
        const test_time_tutil_data_t* entry = &data_entries[idx];

        /* Set the time, call the elapsed, check the outcome */
        time_now = entry->time_now;
        val = lwutil_tutil_has_elapsed(time_now, &time_last, 500U);
        if (val != entry->val_expected) {
            printf("Test failed: Line: %u, val: %u (expected: %u), time_now: %u, time_last: %u (expected: %u), "
                   "data array index: %u",
                   (int)__LINE__, (unsigned)val, (unsigned)entry->val_expected, (unsigned)time_now,
                   (unsigned)time_last, (unsigned)entry->time_expected, (unsigned)idx);
        }
        TEST_IF_TRUE(val == entry->val_expected);
        TEST_IF_TRUE(time_last == entry->time_expected);
    }
    return retval;
}
