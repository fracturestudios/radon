
#include <QHash>
#include <QList>

// Tracks sender-side packets that have not yet been acknowledged.
//
// This object is a utility used by RnSocket.
//
// When RnSocket sends a packet, it also adds the packet to a
// retransmission queue to track its status. 
//
// When an RnSocket is updated, it decrements the timeouts of
// all packets in the retransmit queue, and processes any timed
// out packets for retransmit.
//
class RnRetransmit
{
public:
    // Adds a packet to the retransmission queue
    void add(RnHeader *packet, size_t size, int timeout, int numTries);

    // Updates the time remaining for each item in the queue
    void update(int elapsed);

    // Returns packets for which the timeout has expired
    QList<RnHeader*> timedOut() const;

    // Resets the timeout for the given packet
    void reset(uint32_t seq);

    // Removes the given packet from the retransmission queue
    void remove(uint32_t seq);

private:
    struct Item
    {
        RnHeader *packet;   // The packet payload to (re)send
        size_t size;        // Size of the packet to resend, in bytes
        int timeout;        // Total milliseconds to wait for an ACK
        int numTries;       // Number of times to attempt to send packet
        int remainingTime;  // Milliseconds remaining until next try
    };

    QHash<uint32_t, Item> m_items;
};

