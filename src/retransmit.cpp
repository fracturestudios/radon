
#include <rn/endpoint.h>
#include <rn/header.h>
#include <rn/history.h>
#include <rn/retransmit.h>
#include <rn/socket.h>

void RnRetransmit::add(RnHeader *packet,
                       size_t size,
                       int timeout,
                       int numTries)
{
    Item item;
    item.packet = packet;
    item.size = size;
    item.timeout = timeout;
    item.numTries = numTries;
    item.remainingTime = timeout;

    m_items[packet->seq] = item;
}

void RnRetransmit::update(int elapsed)
{
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        Item &item = it.value();

        if (item.remainingTime > elapsed) {
            item.remainingTime -= elapsed;
        }
        else {
            item.remainingTime = 0;
        }
    }
}

QList<RnHeader*> RnRetransmit::timedOut() const
{
    QList<RnHeader*> result;

    for (auto it = m_items.constBegin(); it != m_items.constEnd(); ++it) {
        const Item &item = it.value();

        if (item.remainingTime == 0) {
            result.append(item.packet);
        }
    }

    return result;
}

void RnRetransmit::reset(uint32_t seq)
{
    if (m_items.contains(seq)) {
        Item &item = m_items[seq];
        item.remainingTime = item.timeout;
    }
}

void RnRetransmit::remove(uint32_t seq)
{
    if (m_items.contains(seq)) {
        m_items.remove(seq);
    }
}

