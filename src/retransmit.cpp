
#include <rn/header.h>
#include <rn/retransmit.h>

void RnRetransmit::add(RnHeader *packet,
                       size_t size,
                       int timeout,
                       int maxRetries)
{
    Item item;
    item.packet = packet;
    item.size = size;
    item.timeout = timeout;
    item.remainingTime = timeout;
    item.remainingTries = maxRetries;

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

QList<RnRetransmit::Item> RnRetransmit::expired() const
{
    QList<RnRetransmit::Item> result;

    for (auto it = m_items.constBegin(); it != m_items.constEnd(); ++it) {
        const Item &item = it.value();

        if (item.remainingTime == 0) {
            result.append(item);
        }
    }

    return result;
}

void RnRetransmit::reset(const Item &item)
{
    uint16_t seq = item.packet->seq;

    if (m_items.contains(seq)) {
        Item &item = m_items[seq];
        item.remainingTime = item.timeout;
    }
}

void RnRetransmit::remove(const Item &item)
{
    uint16_t seq = item.packet->seq;

    if (m_items.contains(seq)) {
        m_items.remove(seq);
    }
}

