cmake_minimum_required(VERSION 2.8.5)

project(learning) 

add_executable(modbus_test)
target_sources(modbus_test PRIVATE modbus_test.c) 
target_link_libraries(modbus_test modbus)

add_executable(udp_test)
target_sources(udp_test PRIVATE udp_test.c) 

add_executable(tcp_test)
target_sources(tcp_test PRIVATE tcp_test.c)

add_executable(thread_test)
target_sources(thread_test PRIVATE thread_test.c)
target_link_libraries(thread_test pthread)

add_executable(file_test)
target_sources(file_test PRIVATE file_test.c)

add_executable(parse_test)
target_sources(parse_test PRIVATE getopt_long_test.c)

add_executable(msg_test)
target_sources(msg_test PRIVATE msg_test.c)
target_link_libraries(msg_test pthread)

add_executable(va_test)
target_sources(va_test PRIVATE va_test.c)