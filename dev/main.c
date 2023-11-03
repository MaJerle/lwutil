#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "lwutil/lwutil.h"

#define TEST_IF_TRUE(condition)                                                                                        \
    if (!(condition)) {                                                                                                \
        printf("Condition %s failed on line %d\r\n", #condition, (int)__LINE__);                                       \
    }

int
main(void) {
    /* Test storing integer device */
    {
        uint8_t arr[10];

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u16_le(0x1234, arr);
        TEST_IF_TRUE(arr[0] == 0x34 && arr[1] == 0x12 && arr[2] == 0x00);

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u16_be(0x1234, arr);
        TEST_IF_TRUE(arr[0] == 0x12 && arr[1] == 0x34 && arr[2] == 0x00);

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u32_le(0x12345678, arr);
        TEST_IF_TRUE(arr[0] == 0x78 && arr[1] == 0x56 && arr[2] == 0x34 && arr[3] == 0x12 && arr[4] == 0x00);

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u32_be(0x12345678, arr);
        TEST_IF_TRUE(arr[3] == 0x78 && arr[2] == 0x56 && arr[1] == 0x34 && arr[0] == 0x12 && arr[4] == 0x00);
    }
    /* Test loading integer device */
    {
        uint8_t arr[] = {0x12, 0x34, 0x56, 0x78};
        uint32_t u32;
        uint16_t u16;

        u16 = lwutil_ld_u16_le(arr);
        TEST_IF_TRUE(u16 == 0x3412);
        u16 = lwutil_ld_u16_be(arr);
        TEST_IF_TRUE(u16 == 0x1234);

        u32 = lwutil_ld_u32_le(arr);
        TEST_IF_TRUE(u32 == 0x78563412);
        u32 = lwutil_ld_u32_be(arr);
        TEST_IF_TRUE(u32 == 0x12345678);
    }
    /* Bit set/reset */
    {
        uint32_t val;
        val = lwutil_bits_set(0x12340, 0x01);
        TEST_IF_TRUE(val == 0x12341);

        val = lwutil_bits_clear(0x12341, 0x01);
        TEST_IF_TRUE(val == 0x12340);

        val = lwutil_bits_toggle(0x1234, 0xFF);
        TEST_IF_TRUE(val == (0x1234 ^ 0xFF));
    }
    /* ASCII operations */
    {
        char arr[10];
        lwutil_u32_to_8asciis(0x12345678, (char*)arr);
        TEST_IF_TRUE(strcmp(arr, "12345678") == 0);
        lwutil_u16_to_4asciis(0x1256, (char*)arr);
        TEST_IF_TRUE(strcmp(arr, "1256") == 0);
    }
    /* Test variable integer */
    {
        uint8_t arr[10] = {0};
        uint32_t val;
        size_t len;

        /* 150 encoded as varint, little endian */
        arr[0] = 0x96;
        arr[1] = 0x01;
        len = lwutil_ld_u32_varint(arr, sizeof(arr), &val);
        TEST_IF_TRUE(val == 150 && len == 2);

        /* 86942 encoded as varint, little endian */
        arr[0] = 0x9E;
        arr[1] = 0xA7;
        arr[2] = 0x05;
        len = lwutil_ld_u32_varint(arr, sizeof(arr), &val);
        TEST_IF_TRUE(val == 86942 && len == 3);

        /* Store value */
        len = lwutil_st_u32_varint(150, arr, sizeof(arr));
        TEST_IF_TRUE(arr[0] == 0x96 && arr[1] == 0x01 && len == 2);

        /* Store value */
        len = lwutil_st_u32_varint(86942, arr, sizeof(arr));
        TEST_IF_TRUE(arr[0] == 0x9E && arr[1] == 0xA7 && arr[2] == 0x05 && len == 3);
    }

    return 0;
}