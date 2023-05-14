
add_executable(log_upload file_wr.c main.c udp.c)
target_include_directories(log_upload PRIVATE ./)
target_link_libraries(log_upload pthread)
