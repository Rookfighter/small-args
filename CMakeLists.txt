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