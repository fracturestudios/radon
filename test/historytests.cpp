
#include <rn/history.h>

#include "historytests.h"
#include "test.h"

bool RnHistoryTracking()
{
    RnHistory history;

    RnAssert(history.start() == 0);
    RnAssert(history.end() < 32);

    for (uint32_t i = 0; i < 32; ++i) {
        RnAssert(!history.hasReceived(i));
    }

    for (uint32_t i = 0; i < 32; i += 2) {
        history.markReceived(i);
    }

    for (uint32_t i = 0; i < 32; ++i) {
        if (i % 2) {
            RnAssert(!history.hasReceived(i));
        }
        else {
            RnAssert(history.hasReceived(i));
        }
    }

    return true;
}

bool RnHistoryAdvance()
{
    RnHistory history;

    RnAssert(history.start() == 0);
    RnAssert(history.end() < 32);

    history.markReceived(31);

    RnAssert(!history.hasReceived(0));
    RnAssert(history.hasReceived(31));

    for (uint32_t i = 0; i < 32; ++i) {
        for (uint32_t j = history.start(); j <= history.end(); ++j) {
            if (j == 31) {
                RnAssert(history.hasReceived(j));
            }
            else {
                RnAssert(!history.hasReceived(j));
            }
        }

        history.advanceTo(i + 32);
    }

    return true;
}

