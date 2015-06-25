
#include <rn/header.h>
#include <rn/inbound.h>

void RnInbound::add(RnHeader *packet, size_t size)
{
    Item item;
    item.packet = packet;
    item.size = size;

    int stream = packet->stream;
    if (!m_queues.contains(stream)) {
        m_queues[stream] = QQueue<Item>();
    }

    m_queues[stream].enqueue(item);
}

bool RnInbound::takeNext(int stream, RnHeader **packet, size_t *size)
{
    if (!m_queues.contains(stream)) {
        return false;
    }

    if (m_queues[stream].empty()) {
        return false;
    }

    Item item = m_queues[stream].dequeue();
    *packet = item.packet;
    *size = item.size;

    return true;
}

