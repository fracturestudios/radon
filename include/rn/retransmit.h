
#include <QHash>
#include <QList>

#include <cstdint>

#pragma once

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
    struct Item
    {
        RnHeader *packet;   // The packet payload to (re)send
        size_t size;        // Size of the packet to resend, in bytes
        int timeout;        // Total milliseconds to wait for an ACK
        int remainingTime;  // Milliseconds remaining until next try
        int remainingTries; // Number of remaining times to resend packet
    };

    // Adds a packet to the retransmission queue
    void add(RnHeader *packet, size_t size, int timeout, int maxRetries);

    // Updates the time remaining for each item in the queue
    void update(int elapsed);

    // Returns packets for which the timeout has expired
    QList<Item> expired() const;

    // Resets the timeout for the given packet
    void reset(const Item &item);

    // Removes the given packet from the retransmission queue
    void remove(const Item &item);

private:
    QHash<uint32_t, Item> m_items;
};

