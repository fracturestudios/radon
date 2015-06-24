
// Tracks receiver-side packet acknowledgement history
//
// This object is a utility used by RnSocket.
//
// When RnSocket sends packets, this object is used to retrieve the
// ACK history and write the ack and history fields in the packet
// header.
//
// When RnSocket receives packets, this object is used to check whether
// the packet has already been received, and marks it to be acknowledged.
//
class RnHistory
{
    // TODO
};

