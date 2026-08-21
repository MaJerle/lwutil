#include "lwutil/lwutil.h"
#include "../test.h"

int
test_run(void) {
    int retval = 0;

    uint32_t val;
    val = lwutil_bits_set(0x12340U, 0x01U);
    TEST_IF_TRUE(val == 0x12341U);

    val = lwutil_bits_clear(0x12341U, 0x01U);
    TEST_IF_TRUE(val == 0x12340U);

    val = lwutil_bits_toggle(0x1234U, 0xFFU);
    TEST_IF_TRUE(val == (0x1234U ^ 0xFFU));

    TEST_IF_TRUE(lwutil_bits_is_set_all(0x0FU, 0x0FU) == 1);
    TEST_IF_TRUE(lwutil_bits_is_set_all(0x0FU, 0x1FU) == 0);
    TEST_IF_TRUE(lwutil_bits_is_set_any(0x0FU, 0x10U) == 0);
    TEST_IF_TRUE(lwutil_bits_is_set_any(0x0FU, 0x18U) == 1);

    return retval;
}
