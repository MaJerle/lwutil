#ifndef TEST_COMMON_HDR_H
#define TEST_COMMON_HDR_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define RUN_TEST(x)                                                                                                    \
    do {                                                                                                               \
        if (!(x)) {                                                                                                    \
            printf("Test FAILED on line %u with condition " #x "\r\n", (unsigned)__LINE__);                            \
            return -1;                                                                                                 \
        }                                                                                                              \
    } while (0)
#define FLT_IS_EQUAL(x, y) (fabs((double)(x) - (double)(y)) < 0.00001)
#define INT_IS_EQUAL(x, y) ((int)((x) == (y)))

/* Reports the failure but keeps running, so a single test_run() call surfaces every failure, not just the first */
#define TEST_IF_TRUE(condition)                                                                                       \
    if (!(condition)) {                                                                                               \
        printf("Condition %s failed on %s:%d\r\n", #condition, __FILE__, (int)__LINE__);                              \
        retval = -1;                                                                                                  \
    }

#endif /* TEST_COMMON_HDR_H */
