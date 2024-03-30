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
        uint8_t arr[10U];

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u16_le(0x1234U, arr);
        TEST_IF_TRUE(arr[0] == 0x34U && arr[1] == 0x12U && arr[2] == 0x00U);

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u16_be(0x1234, arr);
        TEST_IF_TRUE(arr[0] == 0x12U && arr[1] == 0x34U && arr[2] == 0x00U);

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u32_le(0x12345678U, arr);
        TEST_IF_TRUE(arr[0] == 0x78U && arr[1] == 0x56U && arr[2] == 0x34U && arr[3] == 0x12U && arr[4] == 0x00U);

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u32_be(0x12345678, arr);
        TEST_IF_TRUE(arr[3] == 0x78U && arr[2] == 0x56U && arr[1] == 0x34U && arr[0] == 0x12 && arr[4] == 0x00U);
    }
    /* Test loading integer device */
    {
        uint8_t arr[] = {0x12U, 0x34U, 0x56U, 0x78U};
        uint32_t u32;
        uint16_t u16;

        u16 = lwutil_ld_u16_le(arr);
        TEST_IF_TRUE(u16 == 0x3412U);
        u16 = lwutil_ld_u16_be(arr);
        TEST_IF_TRUE(u16 == 0x1234U);

        u32 = lwutil_ld_u32_le(arr);
        TEST_IF_TRUE(u32 == 0x78563412U);
        u32 = lwutil_ld_u32_be(arr);
        TEST_IF_TRUE(u32 == 0x12345678U);
    }
    /* Bit set/reset */
    {
        uint32_t val;
        val = lwutil_bits_set(0x12340U, 0x01U);
        TEST_IF_TRUE(val == 0x12341U);

        val = lwutil_bits_clear(0x12341U, 0x01U);
        TEST_IF_TRUE(val == 0x12340U);

        val = lwutil_bits_toggle(0x1234U, 0xFFU);
        TEST_IF_TRUE(val == (0x1234U ^ 0xFFU));
    }
    /* ASCII operations */
    {
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
    }
    /* Test variable integer */
    {
        uint8_t arr[10] = {0};
        uint32_t val;
        size_t len;

        /* 150 encoded as varint, little endian */
        arr[0] = 0x96U;
        arr[1] = 0x01U;
        len = lwutil_ld_u32_varint(arr, sizeof(arr), &val);
        TEST_IF_TRUE(val == 150 && len == 2);

        /* 86942 encoded as varint, little endian */
        arr[0] = 0x9EU;
        arr[1] = 0xA7U;
        arr[2] = 0x05U;
        len = lwutil_ld_u32_varint(arr, sizeof(arr), &val);
        TEST_IF_TRUE(val == 86942U && len == 3);

        /* 86942 encoded as varint, little endian */
        /* Test array too short */
        arr[0] = 0x9EU;
        arr[1] = 0xA7U;
        arr[2] = 0x05U;
        len = lwutil_ld_u32_varint(arr, 2, &val);
        TEST_IF_TRUE(len == 0 && val == 0);

        /* Store value */
        len = lwutil_st_u32_varint(150U, arr, sizeof(arr));
        TEST_IF_TRUE(arr[0] == 0x96 && arr[1] == 0x01 && len == 2);

        /* Store value */
        len = lwutil_st_u32_varint(86942U, arr, sizeof(arr));
        TEST_IF_TRUE(arr[0] == 0x9EU && arr[1] == 0xA7U && arr[2] == 0x05U && len == 3);

        /* Store value */
        len = lwutil_st_u32_varint(86942U, arr, 2);
        TEST_IF_TRUE(len == 0);
    }
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
    printf("Done\r\n");
    return 0;
}