// Copyright (c) 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef QUICHE_QUIC_PLATFORM_API_QUIC_IOVEC_H_
#define QUICHE_QUIC_PLATFORM_API_QUIC_IOVEC_H_

#include <cstddef>
#include <type_traits>

#if defined(OS_WIN) || defined(OS_NACL)
/* Structure for scatter/gather I/O.  */
struct iovec {
  void* iov_base; /* Pointer to data.  */
  size_t iov_len; /* Length of data.  */
};
#else
#include <sys/uio.h>
#endif  // defined(OS_WIN) || defined(OS_NACL)
namespace quic {

// The impl header has to export struct iovec, or a POSIX-compatible polyfill.
// Below, we mostly assert that what we have is appropriate.
static_assert(std::is_standard_layout<struct iovec>::value,
              "iovec has to be a standard-layout struct");

static_assert(offsetof(struct iovec, iov_base) < sizeof(struct iovec),
              "iovec has to have iov_base");
static_assert(offsetof(struct iovec, iov_len) < sizeof(struct iovec),
              "iovec has to have iov_len");

}  // namespace quic

#endif  // QUICHE_QUIC_PLATFORM_API_QUIC_IOVEC_H_