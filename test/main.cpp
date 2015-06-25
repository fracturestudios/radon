
#include "test.h"

#include "historytests.h"
#include "inboundtests.h"

#define DefineTestFunc(func){ #func, func }

static struct
{
    const char *name;
    RnTestFunc  func;
}
g_tests[] =
{
    DefineTestFunc(RnInboundOrdering),
    DefineTestFunc(RnHistoryTracking),
    DefineTestFunc(RnHistoryAdvance),
};

int main(int, const char *[])
{
    size_t numSucceeded = 0;
    size_t numTests = sizeof(g_tests) / sizeof(g_tests[0]);

    for (size_t i = 0; i < numTests; ++i) {
        auto test = g_tests[i];

        printf("[%zu/%zu] %s... ", i + 1, numTests, test.name);
        if (test.func()) {
            printf("PASS\n");
            ++numSucceeded;
        }
        else {
            printf("FAIL\n");
        }
    }

    printf("\n");
    printf("Finished\n");
    printf("%zu tests ran\n", numTests);
    printf("%u%% passed\n", (unsigned)(100.f * numSucceeded / numTests));

    return 0;
}

