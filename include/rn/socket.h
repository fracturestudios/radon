
#include <QList>
#include <QString>

class RnEndpoint;
class RnRetransmit;
class RnHistory;

// When passed as a max retransmission count, indicates the packet
// should be resent until it is acknowledged
//
static const int RADON_INFINITE_RETRIES = 0xffffffff;

// Packet retransmission options for RnSocket
struct RnSocketOpt
{
    int Timeout;        // Time until retransmit, in milliseconds
    int MaxRetransmits; // Max number of times to resend each packet
};

// A socket which implements the Radon reliable UDP protocol
class RnSocket
{
public:
    // Updates all active RnSocket objects and processes any packet
    // retransmissions required due to the time elapsed.
    //
    // The client should call this method regularly (e.g., in a game
    // or simulation, once per logical update tick). If this method
    // is not called, no packets will ever be retransmitted.
    //
    static void update(int elapsed);

    RnSocket(RnEndpoint *endpoint, const RnSocketOpt &opt);
    ~RnSocket();

    // Gets or sets the time, in milliseconds, to wait before resending
    // packets which have not been acknowledged in the interim
    //
    int retransmitTimeout() const;
    void setRetransmitTimeout(int timeout);

    // The maximum number of times to retransmit each lost packet
    int maxRetransmits() const;
    void setMaxRetransmits(int max);

    // Sends a packet containing the given data to the remote endpoint
    void send(const uint8_t *data, size_t size, int stream = 0);

    // Tries to receive a packet, and returns a bool indicating whether
    // a packet was returned.
    //
    // If a packet is returned, the data buffer should be freed using
    // delete[].
    //
    bool recv(uint8_t **data, size_t *size, int *stream = 0);

private:
    RnEndpoint *m_endpoint;
    RnRetransmit *m_retransmit;
    RnHistory *m_ack;
    uint32_t m_seq;
};

