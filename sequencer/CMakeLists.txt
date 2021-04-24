set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR}/)
set(sequencer_LIB
${CMAKE_SOURCE_DIR}/sequencer/quic_error_codes.cc
${CMAKE_SOURCE_DIR}/sequencer/quic_stream_frame.cc
${CMAKE_SOURCE_DIR}/sequencer/quic_stream_sequencer.cc
${CMAKE_SOURCE_DIR}/sequencer/quic_stream_sequencer_buffer.cc
)
add_library(sequencer STATIC ${sequencer_LIB})