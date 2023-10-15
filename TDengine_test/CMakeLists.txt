project(sync_test)

#link_directories(sync_test /usr/local/taos/driver)
add_executable(sync_test ./sync_query.c)
target_link_libraries(sync_test taos)
