#pragma once
#include "quic_types.h"
namespace quic{
    const QuicPacketCount kMaxPacketGap = 5000;
    const QuicByteCount kStreamReceiveWindowLimit = 64 * 1024 * 1024;   // 16 MB
}
