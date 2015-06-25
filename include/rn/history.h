
#include <cstdint>

#pragma once

struct RnHeader;

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
public:
    RnHistory();
    ~RnHistory();

    uint32_t start() const;
    uint32_t end() const;

    void advanceTo(uint32_t seq);

    bool hasReceived(uint32_t seq) const;
    void markReceived(uint32_t seq);

    void fillHeader(RnHeader *header) const;

private:
    uint32_t m_start;
    uint32_t m_values;
};

