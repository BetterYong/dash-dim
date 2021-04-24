#pragma once
#include <stdint.h>
namespace quic{
using QuicPacketLength = uint16_t;
using QuicStreamId = uint32_t;

// Count of stream IDs. Used in MAX_STREAMS and STREAMS_BLOCKED frames.
using QuicStreamCount = QuicStreamId;

using QuicByteCount = uint64_t;
using QuicPacketCount = uint64_t;
using QuicStreamOffset = uint64_t;
enum QuicFrameType : uint8_t {
  CRYPTO_FRAME = 8,
  STREAM_FRAME=9,
  NUM_FRAME_TYPES
};    
}
