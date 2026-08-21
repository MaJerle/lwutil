#include <string.h>
#include "lwutil/lwutil.h"
#include "../test.h"

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
    /*
     * Regression guard for the signed left-shift overflow fix: the byte that lands
     * in the top-most shifted position (<<8 for 16-bit, <<24 for 32-bit) must have
     * its top bit set, since that is exactly what triggered the undefined behavior.
     */
    {
        uint8_t arr_ff16[2] = {0xFFU, 0xFFU};
        uint8_t arr_ff32[4] = {0xFFU, 0xFFU, 0xFFU, 0xFFU};
        uint8_t arr_le16[2] = {0x01U, 0xFFU};
        uint8_t arr_be16[2] = {0xFFU, 0x01U};
        uint8_t arr_le32[4] = {0x01U, 0x02U, 0x03U, 0xFFU};
        uint8_t arr_be32[4] = {0xFFU, 0x02U, 0x03U, 0x04U};
        uint8_t arr[4];

        /* All-max-value round trip */
        TEST_IF_TRUE(lwutil_ld_u16_le(arr_ff16) == 0xFFFFU);
        TEST_IF_TRUE(lwutil_ld_u16_be(arr_ff16) == 0xFFFFU);
        TEST_IF_TRUE(lwutil_ld_u32_le(arr_ff32) == 0xFFFFFFFFU);
        TEST_IF_TRUE(lwutil_ld_u32_be(arr_ff32) == 0xFFFFFFFFU);

        /* Byte with the top bit set placed specifically in the shifted position */
        TEST_IF_TRUE(lwutil_ld_u16_le(arr_le16) == 0xFF01U);
        TEST_IF_TRUE(lwutil_ld_u16_be(arr_be16) == 0xFF01U);
        TEST_IF_TRUE(lwutil_ld_u32_le(arr_le32) == 0xFF030201U);
        TEST_IF_TRUE(lwutil_ld_u32_be(arr_be32) == 0xFF020304U);

        /* Store + load round trip of the max value */
        lwutil_st_u16_le(0xFFFFU, arr);
        TEST_IF_TRUE(lwutil_ld_u16_le(arr) == 0xFFFFU);
        lwutil_st_u32_le(0xFFFFFFFFU, arr);
        TEST_IF_TRUE(lwutil_ld_u32_le(arr) == 0xFFFFFFFFU);
    }
    return retval;
}
