add_executable(sqlite_test main.c sqlite/sqlite3.c)
target_link_libraries (sqlite_test pthread dl)
target_include_directories(sqlite_test PRIVATE sqlite)