
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
    // TODO
};

