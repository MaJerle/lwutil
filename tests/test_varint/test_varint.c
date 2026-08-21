#include <string.h>
#include "lwutil/lwutil.h"
#include "../test.h"

int
test_run(void) {
    int retval = 0;

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
    return retval;
}
