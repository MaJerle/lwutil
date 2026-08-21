#include <string.h>
#include "lwutil/lwutil.h"
#include "../test.h"

int
test_run(void) {
    int retval = 0;

    char arr[10];
    lwutil_u32_to_8asciis(0x12345678U, (char*)arr);
    TEST_IF_TRUE(strcmp(arr, "12345678") == 0);
    lwutil_u32_to_8asciis(0x5678U, (char*)arr);
    TEST_IF_TRUE(strcmp(arr, "00005678") == 0);
    lwutil_u16_to_4asciis(0x1256U, (char*)arr);
    TEST_IF_TRUE(strcmp(arr, "1256") == 0);
    lwutil_u16_to_4asciis(0x156U, (char*)arr);
    TEST_IF_TRUE(strcmp(arr, "0156") == 0);
    lwutil_u8_to_2asciis(0x16U, (char*)arr);
    TEST_IF_TRUE(strcmp(arr, "16") == 0);
    lwutil_u8_to_2asciis(0x6U, (char*)arr);
    TEST_IF_TRUE(strcmp(arr, "06") == 0);

    return retval;
}
