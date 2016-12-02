cmake_minimum_required(VERSION 2.8)

project(SmallArgs)
    
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS "-Wall -Wextra")

include_directories(
    "include"
    "dep"
)

file(GLOB TEST_C_SRC "test/*.c")

add_executable(sarg_test_c ${TEST_C_SRC})
add_executable(sarg_sample_get_c "sample/sample_get.c")
add_executable(sarg_sample_cb_c  "sample/sample_cb.c")
target_link_libraries(sarg_sample_get_c m)
target_link_libraries(sarg_sample_cb_c m)