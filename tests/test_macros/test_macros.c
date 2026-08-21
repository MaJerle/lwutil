#include "lwutil/lwutil.h"
#include "../test.h"

/*
 * Compile-time regression guard: LWUTIL_COMPILE_TIME_ASSERT must actually compile.
 * It previously expanded to a nonexistent LWUTIL_CONCAT2 macro and failed to build
 * whenever used, but nothing in the test suite ever invoked it, so the break went unnoticed.
 */
LWUTIL_COMPILE_TIME_ASSERT(sizeof(uint16_t) == 2, u16_size_check)
LWUTIL_COMPILE_TIME_ASSERT(sizeof(uint32_t) == 4, u32_size_check)

int
test_run(void) {
    int retval = 0;

    /* Test min max constrain */
    {
        uint32_t val;

        val = LWUTIL_MIN(10, 20);
        TEST_IF_TRUE(val == 10);

        val = LWUTIL_MAX(10, 20);
        TEST_IF_TRUE(val == 20);

        val = LWUTIL_MAX(LWUTIL_MIN(10, 20), 30);
        TEST_IF_TRUE(val == 30);

        val = LWUTIL_MIN(LWUTIL_MAX(10, 20), 30);
        TEST_IF_TRUE(val == 20);

        val = LWUTIL_CONSTRAIN(10, 20, 30);
        TEST_IF_TRUE(val == 20);

        val = LWUTIL_CONSTRAIN(20, 10, 30);
        TEST_IF_TRUE(val == 20);

        val = LWUTIL_CONSTRAIN(20, 25, 30);
        TEST_IF_TRUE(val == 25);

        val = LWUTIL_CONSTRAIN(20, 35, 30);
        TEST_IF_TRUE(val == 30);
    }
    /* Test map */
    {
        uint32_t val;

        /* Map positive scale */
        val = LWUTIL_MAP(10, 5, 15, 50, 100);
        TEST_IF_TRUE(val == 75);

        /* Map negative scale */
        val = LWUTIL_MAP(10, 5, 15, 90, 50);
        TEST_IF_TRUE(val == 70);
    }
    /* Test array size and absolute value macros */
    {
        uint32_t arr5[5];
        uint8_t arr9[9];

        TEST_IF_TRUE(LWUTIL_ARRAYSIZE(arr5) == 5U);
        TEST_IF_TRUE(LWUTIL_ASZ(arr9) == 9U);

        TEST_IF_TRUE(LWUTIL_ABS(-5) == 5);
        TEST_IF_TRUE(LWUTIL_ABS(5) == 5);
        TEST_IF_TRUE(LWUTIL_ABS(0) == 0);
    }
    /* Test set value if pointer not NULL */
    {
        int32_t val = -1;

        LWUTIL_SET_VALUE_IF_PTR_NOT_NULL(&val, 42);
        TEST_IF_TRUE(val == 42);

        /* Must not crash or write anywhere when the pointer is NULL */
        LWUTIL_SET_VALUE_IF_PTR_NOT_NULL((int32_t*)NULL, 100);
        TEST_IF_TRUE(val == 42);
    }
    return retval;
}
