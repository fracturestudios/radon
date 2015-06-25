
#include <cstdio>

#pragma once

typedef bool (*RnTestFunc)();

#define RnTrace(sev, ...)                                           \
    do {                                                            \
        printf("%s: %s (%s:%u) - ",                                 \
                sev,                                                \
                __FUNCTION__,                                       \
                __FILE__,                                           \
                __LINE__);                                          \
        printf(__VA_ARGS__);                                        \
        printf("\n");                                               \
    } while (0)

#define RnInfo(...) RnTrace("Info", __VA_ARGS__)
#define RnWarn(...) RnTrace("Warn", __VA_ARGS__)
#define RnFail(...) RnTrace("Fail", __VA_ARGS__)

#define RnAssert(expr)                                              \
    do {                                                            \
        if ((expr)) {                                               \
            RnInfo("Assert: %s [OK]", #expr);                       \
        }                                                           \
        else {                                                      \
            RnFail("Assert: %s [FAIL]", #expr);                     \
            return false;                                           \
        }                                                           \
    } while (0)

