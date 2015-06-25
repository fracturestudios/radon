
#include <rn/endpoint.h>

class RnQUdpEndpoint : public RnEndpoint
{
public:
    RnQUdpEndpoint(QUdpSocket *sock,
                   const QHostAddress &addr,
                   uint16_t port);
    virtual ~RnQUdpEndpoint();

    virtual void send(const uint8_t *data, size_t size) override;
    virtual bool recv(uint8_t **data, size_t *size) override;
    virtual void release() override;

private:
    QUdpSocket  *m_sock;
    QHostAddress m_addr;
    uint16_t     m_port;
};

RnQUdpEndpoint::RnQUdpEndpoint(QUdpSocket *sock,
                               const QHostAddress &addr,
                               uint16_t port)
    : m_sock(sock)
    , m_addr(addr)
    , m_port(port)
{ }

RnQUdpEndpoint::~RnQUdpEndpoint()
{
    delete m_sock;
    m_sock = nullptr;
}

void RnQUdpEndpoint::send(const uint8_t *data, size_t size)
{
    const char *buf = reinterpret_cast<const char*>(data);
    m_sock->writeDatagram(buf, size, m_addr, m_port);
}

bool RnQUdpEndpoint::recv(uint8_t **outdata,  size_t *outsize)
{
    size_t size = m_sock->pendingDatagramSize();
    if (size == 0) {
        return false;
    }

    char *data = new char[size];
    m_sock->readDatagram(data, size);

    *outdata = reinterpret_cast<uint8_t*>(data);
    *outsize = size;

    return true;
}

void RnQUdpEndpoint::release()
{
    delete this;
}

RnEndpoint *RnEndpoint::fromQt(QUdpSocket *sock,
                               const QHostAddress &addr,
                               uint16_t port)
{
    return new RnQUdpEndpoint(sock, addr, port);
}

