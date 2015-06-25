
#include <QHash>
#include <QList>
#include <QQueue>

#include <cstdint>

// Tracks receiver-side packets which have been read fromthe socket
// but not yet delivered to the client.
//
// This object is a utility used by RnSocket.
//
// When inbound packets are processed in RnSocket::update(), packets that
// need to be transmitted to the client are sorted and inserted into this
// object.
//
// Later, when the client calls recv() on the packet, the matching packets
// for the given stream index are removed from this queue and returned to
// the caller.
//
class RnInbound
{
public:
    // Adds a packet to the inbound queue
    void add(RnHeader *packet, size_t size);

    // Retrieves the next packet for the given stream index
    bool takeNext(int stream, RnHeader **packet, size_t *size);

private:
    struct Item
    {
        RnHeader *packet;   // The packet payload to (re)send
        size_t size;        // Size of the packet to resend, in bytes
    };

    QHash<int, QQueue<Item>> m_queues;
};

