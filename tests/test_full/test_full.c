#include <stdio.h>
#include <string.h>
#include "lwutil/lwutil.h"

#define TEST_IF_TRUE(condition)                                                                                        \
    if (!(condition)) {                                                                                                \
        printf("Condition %s failed on line %d\r\n", #condition, (int)__LINE__);                                       \
        retval = -1;                                                                                                   \
    }

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
    /* Bit set/reset */
    {
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
    /* Test variable integer parsed from a byte stream, one after another */
    {
        /* 300 (0xAC, 0x02), then 86942 (0x9E, 0xA7, 0x05), little endian varints back to back */
        uint8_t arr[10] = {0xACU, 0x02U, 0x9EU, 0xA7U, 0x05U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};
        const uint8_t* p = arr;
        size_t remaining = sizeof(arr);
        uint32_t val;
        uint8_t used;

        /* First call sees the whole remaining stream, not just its own varint's bytes */
        used = lwutil_ld_u32_varint(p, remaining, &val);
        TEST_IF_TRUE(used == 2U);
        TEST_IF_TRUE(val == 300U);
        p += used;
        remaining -= used;

        /* Second call continues from where the first one stopped */
        used = lwutil_ld_u32_varint(p, remaining, &val);
        TEST_IF_TRUE(used == 3U);
        TEST_IF_TRUE(val == 86942U);
        p += used;
        remaining -= used;
    }
    /* Test variable integer with malformed (never-ending continuation bit) stream */
    {
        /* Every byte has bit 0x80 set, no valid terminator within (or beyond) the 5-byte limit */
        uint8_t arr[10] = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};
        uint32_t val;
        uint8_t used;

        /* Must be rejected as an error, and must never read/shift past the 5-byte limit */
        used = lwutil_ld_u32_varint(arr, sizeof(arr), &val);
        TEST_IF_TRUE(used == 0U);
        TEST_IF_TRUE(val == 0U);
    }
    /* Test variable integer NULL / zero-length argument handling */
    {
        uint8_t arr[10] = {0x01U};
        uint32_t val = 0xAAAAAAAAU;
        uint8_t used;

        used = lwutil_ld_u32_varint(NULL, sizeof(arr), &val);
        TEST_IF_TRUE(used == 0U);

        used = lwutil_ld_u32_varint(arr, 0, &val);
        TEST_IF_TRUE(used == 0U);

        used = lwutil_ld_u32_varint(arr, sizeof(arr), NULL);
        TEST_IF_TRUE(used == 0U);

        used = lwutil_st_u32_varint(123U, NULL, sizeof(arr));
        TEST_IF_TRUE(used == 0U);

        used = lwutil_st_u32_varint(123U, arr, 0);
        TEST_IF_TRUE(used == 0U);
    }
    /* Test variable integer boundary values, at each 7-bit continuation rollover */
    {
        static const uint32_t boundary_values[] = {
            0U, 127U, 128U, 16383U, 16384U, 2097151U, 2097152U, 268435455U, 268435456U, 0xFFFFFFFFU,
        };
        uint8_t arr[8];
        uint32_t val;
        uint8_t st_len, ld_len;

        for (size_t idx = 0; idx < LWUTIL_ARRAYSIZE(boundary_values); ++idx) {
            memset(arr, 0xCCU, sizeof(arr));
            st_len = lwutil_st_u32_varint(boundary_values[idx], arr, sizeof(arr));
            TEST_IF_TRUE(st_len > 0U && st_len <= 5U);

            val = 0;
            ld_len = lwutil_ld_u32_varint(arr, sizeof(arr), &val);
            TEST_IF_TRUE(ld_len == st_len);
            TEST_IF_TRUE(val == boundary_values[idx]);
        }

        /* Explicit byte-level anchors at both ends of the range */
        memset(arr, 0xCCU, sizeof(arr));
        st_len = lwutil_st_u32_varint(0U, arr, sizeof(arr));
        TEST_IF_TRUE(st_len == 1U);
        TEST_IF_TRUE(arr[0] == 0x00U);

        memset(arr, 0xCCU, sizeof(arr));
        st_len = lwutil_st_u32_varint(0xFFFFFFFFU, arr, sizeof(arr));
        TEST_IF_TRUE(st_len == 5U);
        TEST_IF_TRUE(arr[0] == 0xFFU);
        TEST_IF_TRUE(arr[1] == 0xFFU);
        TEST_IF_TRUE(arr[2] == 0xFFU);
        TEST_IF_TRUE(arr[3] == 0xFFU);
        TEST_IF_TRUE(arr[4] == 0x0FU);
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
    /* Test time period */
    {
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
    }
    /* Test rolling regression slope */
    {
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
    }
    return retval;
}
