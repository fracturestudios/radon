
#include <cstdint>

#pragma once

// Binary representation of a Radon protocol packet header
//
// For details, see README.markdown
//
#if defined(Q_OS_WIN)
#   pragma pack(1)
#endif
struct
#if !defined(Q_OS_WIN)
__attribute__((packed))
#endif
RnHeader
{
    uint16_t magic;     // Magic number (should always be ASCII "Rn")
    uint16_t stream;    // Stream index
    uint32_t seq;       // Packet sequence number
    uint32_t ack;       // Sequence number of ACK window
    uint32_t history;   // Contents of ACK window
};

