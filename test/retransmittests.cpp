
#include <rn/header.h>
#include <rn/retransmit.h>

#include "retransmittests.h"
#include "test.h"

static const size_t PACKET_SIZE = 100;

static RnHeader *makePacket(uint32_t seq)
{
    RnHeader *result = reinterpret_cast<RnHeader*>(
            new uint8_t[PACKET_SIZE]
        );
    memset(result, 0, PACKET_SIZE);

    result->seq = seq;
    return result;
}

bool RnRetransmitExpiry()
{
    RnRetransmit retransmit;

    retransmit.add(makePacket(1), PACKET_SIZE, 100, 1);
    retransmit.add(makePacket(2), PACKET_SIZE, 200, 1);
    retransmit.add(makePacket(3), PACKET_SIZE, 300, 1);

    retransmit.update(50);
    RnAssert(retransmit.expired().empty());

    retransmit.update(100);

    QList<RnRetransmit::Item> items = retransmit.expired();
    RnAssert(items.size() == 1);

    RnAssert(items[0].packet->seq == 1);
    RnAssert(items[0].timeout == 100);
    RnAssert(items[0].remainingTime == 0);
    RnAssert(items[0].remainingTries == 1);

    retransmit.remove(items[0]);

    retransmit.update(100);

    items = retransmit.expired();
    RnAssert(items.size() == 1);

    RnAssert(items[0].packet->seq == 2);
    RnAssert(items[0].timeout == 200);
    RnAssert(items[0].remainingTime == 0);
    RnAssert(items[0].remainingTries == 1);

    retransmit.remove(items[0]);

    retransmit.update(100);

    items = retransmit.expired();
    RnAssert(items.size() == 1);

    RnAssert(items[0].packet->seq == 3);
    RnAssert(items[0].timeout == 300);
    RnAssert(items[0].remainingTime == 0);
    RnAssert(items[0].remainingTries == 1);

    retransmit.remove(items[0]);

    retransmit.update(1000);
    RnAssert(retransmit.expired().empty());

    return true;
}

bool RnRetransmitReset()
{
    RnRetransmit retransmit;

    retransmit.add(makePacket(1), PACKET_SIZE, 100, 1);
    retransmit.update(150);

    QList<RnRetransmit::Item> items = retransmit.expired();
    RnAssert(items.size() == 1);

    RnAssert(items[0].packet->seq == 1);
    RnAssert(items[0].timeout == 100);
    RnAssert(items[0].remainingTime == 0);
    RnAssert(items[0].remainingTries == 1);

    retransmit.reset(items[0]);

    retransmit.update(50);
    RnAssert(retransmit.expired().empty());

    retransmit.update(100);

    items = retransmit.expired();
    RnAssert(items.size() == 1);

    RnAssert(items[0].packet->seq == 1);
    RnAssert(items[0].timeout == 100);
    RnAssert(items[0].remainingTime == 0);
    RnAssert(items[0].remainingTries == 0);

    retransmit.remove(items[0]);
    return true;
}

