cmake_minimum_required(VERSION 2.8)

project(SmallArgs)
    
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS "-Wall -Wextra")

include_directories(
    "include"
    "dep"
)

file(GLOB TEST_C_SRC "test/*.c")
file(GLOB SAMPLE_C_SRC "sample/*.c")

add_executable(sarg_test_c ${TEST_C_SRC})
add_executable(sarg_sample_c ${SAMPLE_C_SRC})
target_link_libraries(sarg_sample_c m)