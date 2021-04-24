PROJECT(project)
cmake_minimum_required(VERSION 2.6)
SET(CMAKE_BUILD_TYPE "Debug")
add_definitions(-std=c++14)
#add_definitions(-DNDEBUG)
include_directories(${CMAKE_SOURCE_DIR}/)
include_directories(${CMAKE_SOURCE_DIR}/logging)
include_directories(${CMAKE_SOURCE_DIR}/base)
include_directories(${CMAKE_SOURCE_DIR}/sequencer)
include_directories(${CMAKE_SOURCE_DIR}/tcp)
#include_directories(${CMAKE_SOURCE_DIR}/dash)
include_directories(${CMAKE_SOURCE_DIR}/tpproxy)
add_subdirectory(base)
#add_subdirectory(dash)
add_subdirectory(sequencer)

set(EXECUTABLE_NAME "t_tcp_server")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/tcp/tcp_server_test.cc)
target_link_libraries(${EXECUTABLE_NAME} base   pthread)
set(EXECUTABLE_NAME "t_tcp_client")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/tcp/tcp_client_test.cc)
target_link_libraries(${EXECUTABLE_NAME} base  pthread)
set(EXECUTABLE_NAME "t_asyn_client")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/tcp/asyn_client_test.cc)
target_link_libraries(${EXECUTABLE_NAME} base  pthread)


#set(EXECUTABLE_NAME "t_dash_server")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/dash/dash_server_test.cc)
#target_link_libraries(${EXECUTABLE_NAME} dash base  pthread)

#set(EXECUTABLE_NAME "t_dash_client")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/dash/dash_client_test.cc)
#target_link_libraries(${EXECUTABLE_NAME} dash base  pthread)

set(EXECUTABLE_NAME "possion_sender")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/tools/possion_sender.cc)
target_link_libraries(${EXECUTABLE_NAME})
set(EXECUTABLE_NAME "f_sender")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/tools/f_sender.cc)
target_link_libraries(${EXECUTABLE_NAME} base pthread)
set(EXECUTABLE_NAME "f_server")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/tools/f_server.cc)
target_link_libraries(${EXECUTABLE_NAME} base pthread)

set(mpproxy_src
tpproxy/interval_budget.cc
tpproxy/mpproxy_client.cc
tpproxy/mpproxy_server.cc
tpproxy/mpproxy_types.cc
tpproxy/tpproxy_server.cc
hen/hen_base.cc
octopus/octopus_base.cc
)
#add_library(proxy STATIC ${mpproxy_src})
set(octopus_src
octopus/arp_server.cc
octopus/octopus_base.cc
)
add_library(oct_base STATIC ${octopus_src})

set(EXECUTABLE_NAME "a_echo")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/hen/a_echo.cc)
target_link_libraries(${EXECUTABLE_NAME} base pthread)
set(EXECUTABLE_NAME "d_echo")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/hen/d_echo.cc)
target_link_libraries(${EXECUTABLE_NAME} base pthread)
#set(EXECUTABLE_NAME "b_hen")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/hen/b_hen.cc)
#target_link_libraries(${EXECUTABLE_NAME} proxy base pthread)
#set(EXECUTABLE_NAME "c_hen")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/hen/c_hen.cc)
#target_link_libraries(${EXECUTABLE_NAME} proxy base pthread)

#set(EXECUTABLE_NAME "oct_client")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/octopus/octopus_client_test.cc)
#target_link_libraries(${EXECUTABLE_NAME} proxy base pthread)
#set(EXECUTABLE_NAME "oct_server")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/octopus/octopus_server_test.cc)
#target_link_libraries(${EXECUTABLE_NAME} proxy base pthread)
set(EXECUTABLE_NAME "oct")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/octopus/octopus.cc)
target_link_libraries(${EXECUTABLE_NAME}  oct_base base pthread)

#set(EXECUTABLE_NAME "arp_request")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/octopus/arp_request.cc)
#target_link_libraries(${EXECUTABLE_NAME}  base)

#set(EXECUTABLE_NAME "arp_reply")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/octopus/arp_capture.cc)
#target_link_libraries(${EXECUTABLE_NAME} oct_base  base pthread)

set(EXECUTABLE_NAME "arp_reply")
add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/octopus/arp_reply.cc)
target_link_libraries(${EXECUTABLE_NAME} oct_base base pthread)
#set(EXECUTABLE_NAME "sproof_tcp")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/hen/sproof_tcp.cc)
#target_link_libraries(${EXECUTABLE_NAME} base)

#set(tpproxy_src
#tpproxy/tpproxy_server_test.cc
#)
#set(EXECUTABLE_NAME "t_proxy")
#add_executable(${EXECUTABLE_NAME} ${tpproxy_src})
#target_link_libraries(${EXECUTABLE_NAME} proxy base  pthread)

#set(EXECUTABLE_NAME "t_mpproxy_client")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/tpproxy/mpproxy_client_test.cc)
#target_link_libraries(${EXECUTABLE_NAME} proxy sequencer base  pthread)

#set(EXECUTABLE_NAME "t_mpproxy_server")
#add_executable(${EXECUTABLE_NAME} ${CMAKE_SOURCE_DIR}/tpproxy/mpproxy_server_test.cc)
#target_link_libraries(${EXECUTABLE_NAME} proxy sequencer base  pthread)
