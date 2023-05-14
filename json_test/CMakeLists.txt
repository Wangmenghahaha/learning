add_executable(json_test main.c cJSON/cJSON.c cJSON/cJSON_Utils.c)
target_include_directories(json_test PRIVATE cJSON)
