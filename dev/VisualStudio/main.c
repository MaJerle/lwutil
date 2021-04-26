#include <stdio.h>
#include "lwutil.h"
#include <string.h>
#include <stdint.h>

int main(void) {
    uint8_t arr[10];

    lwutil_st_u32_le(0x1234, arr);

    uint32_t val = 0x01;
    val = lwutil_bits_set(val, 0x12340);

    printf("%u\r\n", (unsigned)val);

    return 0;
}