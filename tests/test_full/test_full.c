#include <stdio.h>
#include <string.h>
#include "lwutil/lwutil.h"

#define TEST_IF_TRUE(condition)                                                                                        \
    if (!(condition)) {                                                                                                \
        printf("Condition %s failed on line %d\r\n", #condition, (int)__LINE__);                                       \
        retval = -1;                                                                                                   \
    }

int
test_run(void) {
    int retval = 0;

    /* Test storing integer device */
    {
        uint8_t arr[10U];
        uint8_t* ptr;

        /* Standard version */
        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u16_le(0x1234U, arr);
        TEST_IF_TRUE(arr[0] == 0x34U);
        TEST_IF_TRUE(arr[1] == 0x12U);
        TEST_IF_TRUE(arr[2] == 0x00U);

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u16_be(0x1234, arr);
        TEST_IF_TRUE(arr[0] == 0x12U);
        TEST_IF_TRUE(arr[1] == 0x34U);
        TEST_IF_TRUE(arr[2] == 0x00U);

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u32_le(0x12345678U, arr);
        TEST_IF_TRUE(arr[0] == 0x78U);
        TEST_IF_TRUE(arr[1] == 0x56U);
        TEST_IF_TRUE(arr[2] == 0x34U);
        TEST_IF_TRUE(arr[3] == 0x12U);
        TEST_IF_TRUE(arr[4] == 0x00U);

        memset(arr, 0x00, sizeof(arr));
        lwutil_st_u32_be(0x12345678, arr);
        TEST_IF_TRUE(arr[0] == 0x12U);
        TEST_IF_TRUE(arr[1] == 0x34U);
        TEST_IF_TRUE(arr[2] == 0x56U);
        TEST_IF_TRUE(arr[3] == 0x78U);
        TEST_IF_TRUE(arr[4] == 0x00U);

        /* 
         * Extended version accepts the pointer to pointer to the memory,
         * reads the data from the pointer, and then changes the address pointer points too.
         * 
         * This allows the following "load" code:
         * 
         * uint8_t data[10];
         * uint8_t* data_ptr = data;
         * 
         * //Write entries one after another in the data array
         * lwutil_st_u16_le_ex(0x1234, (void**)&data_ptr);
         * lwutil_st_u16_le_ex(0x5678, (void**)&data_ptr);
         *
         * the final data array will look like: 0x34, 0x12, 0x78, 0x56
         */
        memset(arr, 0x00, sizeof(arr));
        ptr = arr;
        lwutil_st_u16_le_ex(0x1234U, (void**)&ptr);
        lwutil_st_u16_le_ex(0x5678U, (void**)&ptr);
        TEST_IF_TRUE(arr[0] == 0x34U);
        TEST_IF_TRUE(arr[1] == 0x12U);
        TEST_IF_TRUE(arr[2] == 0x78U);
        TEST_IF_TRUE(arr[3] == 0x56U);
        TEST_IF_TRUE(arr[4] == 0x00U);
        TEST_IF_TRUE(ptr == &arr[4]);

        memset(arr, 0x00, sizeof(arr));
        ptr = arr;
        lwutil_st_u16_be_ex(0x1234U, (void**)&ptr);
        lwutil_st_u16_be_ex(0x5678U, (void**)&ptr);
        TEST_IF_TRUE(arr[0] == 0x12U);
        TEST_IF_TRUE(arr[1] == 0x34U);
        TEST_IF_TRUE(arr[2] == 0x56U);
        TEST_IF_TRUE(arr[3] == 0x78U);
        TEST_IF_TRUE(arr[4] == 0x00U);
        TEST_IF_TRUE(ptr == &arr[4]);

        memset(arr, 0x00, sizeof(arr));
        ptr = arr;
        lwutil_st_u32_le_ex(0x12345678U, (void**)&ptr);
        lwutil_st_u32_le_ex(0x12345678U, (void**)&ptr);
        TEST_IF_TRUE(arr[0] == 0x78U);
        TEST_IF_TRUE(arr[1] == 0x56U);
        TEST_IF_TRUE(arr[2] == 0x34U);
        TEST_IF_TRUE(arr[3] == 0x12U);
        TEST_IF_TRUE(arr[4] == 0x78U);
        TEST_IF_TRUE(arr[5] == 0x56U);
        TEST_IF_TRUE(arr[6] == 0x34U);
        TEST_IF_TRUE(arr[7] == 0x12U);
        TEST_IF_TRUE(arr[8] == 0x00U);
        TEST_IF_TRUE(ptr == &arr[8]);

        memset(arr, 0x00, sizeof(arr));
        ptr = arr;
        lwutil_st_u32_be_ex(0x12345678, (void**)&ptr);
        lwutil_st_u32_be_ex(0x12345678, (void**)&ptr);
        TEST_IF_TRUE(arr[0] == 0x12);
        TEST_IF_TRUE(arr[1] == 0x34U);
        TEST_IF_TRUE(arr[2] == 0x56U);
        TEST_IF_TRUE(arr[3] == 0x78U);
        TEST_IF_TRUE(arr[4] == 0x12);
        TEST_IF_TRUE(arr[5] == 0x34U);
        TEST_IF_TRUE(arr[6] == 0x56U);
        TEST_IF_TRUE(arr[7] == 0x78U);
        TEST_IF_TRUE(arr[8] == 0x00U);
        TEST_IF_TRUE(ptr == &arr[8]);
    }
    /* Test loading integer device */
    {
        uint8_t arr[] = {0x12U, 0x34U, 0x56U, 0x78U, 0xDEU, 0xADU, 0xBEU, 0xEFU};
        uint8_t* ptr;
        uint32_t u32_1, u32_2;
        uint16_t u16_1, u16_2;

        /* Standard version */
        u16_1 = lwutil_ld_u16_le(arr);
        TEST_IF_TRUE(u16_1 == 0x3412U);
        u16_1 = lwutil_ld_u16_be(arr);
        TEST_IF_TRUE(u16_1 == 0x1234U);
        u32_1 = lwutil_ld_u32_le(arr);
        TEST_IF_TRUE(u32_1 == 0x78563412U);
        u32_1 = lwutil_ld_u32_be(arr);
        TEST_IF_TRUE(u32_1 == 0x12345678U);

        /* Extended version */

        /* 16-bit */
        ptr = arr;
        u16_1 = lwutil_ld_u16_le_ex((const void**)&ptr);
        u16_2 = lwutil_ld_u16_le_ex((const void**)&ptr);
        TEST_IF_TRUE(u16_1 == 0x3412U);
        TEST_IF_TRUE(u16_2 == 0x7856);
        TEST_IF_TRUE(ptr == &arr[4]);

        ptr = arr;
        u16_1 = lwutil_ld_u16_be_ex((const void**)&ptr);
        u16_2 = lwutil_ld_u16_be_ex((const void**)&ptr);
        TEST_IF_TRUE(u16_1 == 0x1234U);
        TEST_IF_TRUE(u16_2 == 0x5678);
        TEST_IF_TRUE(ptr == &arr[4]);

        /* 32-bit */
        ptr = arr;
        u32_1 = lwutil_ld_u32_le_ex((const void**)&ptr);
        u32_2 = lwutil_ld_u32_le_ex((const void**)&ptr);
        TEST_IF_TRUE(u32_1 == 0x78563412U);
        TEST_IF_TRUE(u32_2 == 0xEFBEADDE);
        TEST_IF_TRUE(ptr == &arr[8]);

        ptr = arr;
        u32_1 = lwutil_ld_u32_be_ex((const void**)&ptr);
        u32_2 = lwutil_ld_u32_be_ex((const void**)&ptr);
        TEST_IF_TRUE(u32_1 == 0x12345678U);
        TEST_IF_TRUE(u32_2 == 0xDEADBEEFU);
        TEST_IF_TRUE(ptr == &arr[8]);
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
        TEST_IF_TRUE(val == 150);
        TEST_IF_TRUE(len == 2);

        /* 86942 encoded as varint, little endian */
        arr[0] = 0x9EU;
        arr[1] = 0xA7U;
        arr[2] = 0x05U;
        len = lwutil_ld_u32_varint(arr, sizeof(arr), &val);
        TEST_IF_TRUE(val == 86942U);
        TEST_IF_TRUE(len == 3);

        /* 86942 encoded as varint, little endian */
        /* Test array too short */
        arr[0] = 0x9EU;
        arr[1] = 0xA7U;
        arr[2] = 0x05U;
        len = lwutil_ld_u32_varint(arr, 2, &val);
        TEST_IF_TRUE(len == 0);
        TEST_IF_TRUE(val == 0);

        /* Store value */
        len = lwutil_st_u32_varint(150U, arr, sizeof(arr));
        TEST_IF_TRUE(arr[0] == 0x96);
        TEST_IF_TRUE(arr[1] == 0x01);
        TEST_IF_TRUE(len == 2);

        /* Store value */
        len = lwutil_st_u32_varint(86942U, arr, sizeof(arr));
        TEST_IF_TRUE(arr[0] == 0x9EU);
        TEST_IF_TRUE(arr[1] == 0xA7U);
        TEST_IF_TRUE(arr[2] == 0x05U);
        TEST_IF_TRUE(len == 3);

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
    return retval;
}