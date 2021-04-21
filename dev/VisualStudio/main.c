#include <stdio.h>
#include "lwutil.h"
#include <string.h>
#include <stdint.h>

int main(void) {
    uint8_t arr[10];

    lwutil_st_u32_le(0x1234, arr);

    return 0;
}