
#include <rn/endpoint.h>
#include <rn/header.h>
#include <rn/history.h>
#include <rn/inbound.h>
#include <rn/retransmit.h>
#include <rn/socket.h>

static const uint16_t RADON_MAGIC = ((uint16_t)'R' << 8) + (uint16_t)'n';

static QList<RnSocket*> g_activeSockets;

RnSocket::RnSocket(RnEndpoint *endpoint, const RnSocketOpt &opt)
    : m_endpoint(endpoint)
    , m_retransmit(new RnRetransmit)
    , m_history(new RnHistory)
    , m_inbound(new RnInbound)
    , m_seq(0)
    , m_timeout(opt.Timeout)
    , m_maxRetries(opt.MaxRetransmits)
{
    g_activeSockets.append(this);
}

RnSocket::~RnSocket()
{
    g_activeSockets.removeOne(this);

    m_endpoint->release();
    m_endpoint = nullptr;

    delete m_retransmit;
    m_retransmit = nullptr;

    delete m_history;
    m_history = nullptr;

    delete m_inbound;
    m_inbound = nullptr;
}

int RnSocket::retransmitTimeout() const
{
    return m_timeout;
}

void RnSocket::setRetransmitTimeout(int timeout)
{
    m_timeout = timeout;
}

int RnSocket::maxRetransmits() const
{
    return m_maxRetries;
}

void RnSocket::setMaxRetransmits(int maxRetransmits)
{
    m_maxRetries = maxRetransmits;
}

void RnSocket::send(const uint8_t *data, size_t size, int stream)
{
    size_t packetSize = sizeof(RnHeader) + size;
    uint8_t *packet = new uint8_t[packetSize];

    RnHeader *header = reinterpret_cast<RnHeader*>(packet);
    header->magic = RADON_MAGIC;
    header->stream = stream;
    header->seq = m_seq++;
    m_history->fillHeader(header);

    memcpy(packet + sizeof(RnHeader), data, size);

    m_endpoint->send(packet, packetSize);
    m_retransmit->add(header, packetSize, m_timeout, m_maxRetries);
}

bool RnSocket::recv(uint8_t **data, size_t *size, int stream)
{
    RnHeader *packet = nullptr;
    size_t packetSize = 0;

    if (m_inbound->takeNext(stream, &packet, &packetSize)) {
        *data = reinterpret_cast<uint8_t*>(packet) + sizeof(RnHeader);
        *size = packetSize - sizeof(RnHeader);

        return true;
    }

    return false;
}

void RnSocket::update(int elapsed)
{
    for (auto is = g_activeSockets.begin();
         is != g_activeSockets.end();
         ++is) {
        
        RnSocket *sock = *is;

        // Resend or drop any timed-out packets in the retransmit queue
        sock->m_retransmit->update(elapsed);

        QList<RnRetransmit::Item> expired = sock->m_retransmit->expired();
        for (auto ii = expired.begin(); ii != expired.end(); ++ii) {
            const RnRetransmit::Item &item = *ii;

            if (item.remainingTries > 0) {
                uint8_t *buf = reinterpret_cast<uint8_t*>(item.packet);
                sock->m_endpoint->send(buf, item.size);

                sock->m_retransmit->reset(item);
            }
            else {
                sock->m_retransmit->remove(item);
            }
        }

        // Receive incoming packets into the inbound queue
        uint8_t *buf = nullptr;
        size_t bufsize = 0;

        while (sock->m_endpoint->recv(&buf, &bufsize)) {

            if (bufsize < sizeof(RnHeader)) { // Invalid format
                delete[] buf;
                buf = nullptr;
                continue;
            }

            RnHeader *header = reinterpret_cast<RnHeader*>(buf);
            if (header->magic != RADON_MAGIC) { // Also invalid format
                delete[] buf;
                buf = nullptr;
                continue;
            }

            uint32_t seq = header->seq;
            if (seq < sock->m_history->start()) { // Packet way too old
                delete[] buf;
                buf = nullptr;
                continue;
            }
            
            sock->m_history->advanceTo(seq);
            if (sock->m_history->hasReceived(seq)) { // Already received
                delete[] buf;
                buf = nullptr;
                continue;
            }

            sock->m_inbound->add(header, bufsize);
            sock->m_history->markReceived(seq);
        }
    }
}

