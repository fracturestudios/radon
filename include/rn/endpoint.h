
#include <QHostAddress>
#include <QUdpSocket>

// Wraps an underlying UDP socket to be used in an RnSocket.
//
// Typical usage is to obtain your own QUdpSocket through your own
// means, use RnEndpoint::fromQt() to create an RnEndpoint which
// wraps the QUdpSocket, and finally passes the result to a new instance
// of RnSocket.
//
class RnEndpoint
{
public:
    // Create an RnEndpoint instance which owns the given QUdpSocket
    // and uses it to send to and receive from the given address.
    //
    static RnEndpoint *fromQt(QUdpSocket *udpSocket,
                              const QHostAddress &address,
                              uint16_t port);

    // Sends a datagram containing the given payload data to the
    // remote endpoint
    //
    virtual void send(const uint8_t *data, size_t size) = 0;

    // Tries to receive a datagram, and returns a bool indicating whether
    // a datagram was returned.
    //
    // If a datagram is returned, the data buffer should be freed using
    // delete[].
    //
    virtual bool recv(uint8_t **data, size_t *size) = 0;
};

