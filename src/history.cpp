
#include <rn/header.h>
#include <rn/history.h>

RnHistory::RnHistory()
    : m_start(0)
    , m_values(0)
{ }

RnHistory::~RnHistory() { }

uint32_t RnHistory::start() const
{
    return m_start;
}

uint32_t RnHistory::end() const
{
    return m_start + 31;
}

void RnHistory::advanceTo(uint32_t seq)
{
    uint32_t end = this->end();

    if (seq > end) {
        uint32_t diff = seq - end;

        m_start += diff;
        m_values = m_values >> diff;
    }
}

bool RnHistory::hasReceived(uint32_t seq) const
{
    if (seq < start() || seq > end()) {
        return false;
    }

    uint32_t bit = seq - start();
    return (m_values & (1 << bit)) != 0;
}

void RnHistory::markReceived(uint32_t seq)
{
    if (seq < start() || seq > end()) {
        return;
    }

    uint32_t bit = seq - start();
    m_values |= (1 << bit);
}

void RnHistory::fillHeader(RnHeader *header) const
{
    header->ack = m_start;
    header->history = m_values;
}

