
#include <rn/header.h>
#include <rn/inbound.h>

#include "inboundtests.h"
#include "test.h"

bool RnInboundOrdering()
{
    static const size_t bufsize = 1024;
    static const size_t msgcount = 3;

    RnInbound inboundQueue;

    for (size_t i = 0; i < msgcount; ++i) {

        uint8_t *buf = new uint8_t[bufsize];
        memset(buf, 0, bufsize);

        for (size_t j = 0; j < bufsize - sizeof(RnHeader); ++j) {
            buf[sizeof(RnHeader) + j] = (uint8_t)i;
        }

        inboundQueue.add(reinterpret_cast<RnHeader*>(buf), bufsize);
    }

    for (size_t i = 0; i < msgcount; ++i) {

        RnHeader *packet = nullptr;
        size_t packetSize = 0;

        RnAssert(inboundQueue.takeNext(0, &packet, &packetSize));
        RnAssert(packet != nullptr);
        RnAssert(packetSize == bufsize);

        uint8_t *buf = reinterpret_cast<uint8_t*>(packet);
        for (size_t j = 0; j < bufsize - sizeof(RnHeader); ++j) {
            RnAssert(buf[sizeof(RnHeader) + j] == i);
        }

        delete[] buf;
        buf = nullptr;
        packet = nullptr;
    }

    return true;
}

