cmake_minimum_required(VERSION 2.8)

project(SmallArgs)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS "-Wall -Wextra")

get_filename_component(CATCH_ROOT "dep/catch" ABSOLUTE)
get_filename_component(CTEST_ROOT "dep/ctest" ABSOLUTE)

get_filename_component(LOCAL_MODULE_PATH "./cmake-modules" ABSOLUTE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${LOCAL_MODULE_PATH})

find_package(Catch)
find_package(CTest)

include_directories(
    "include"
    ${CATCH_INCLUDE_DIR}
    ${CTEST_INCLUDE_DIR}
)

file(GLOB TEST_C_SRC "test/*.c")

add_executable(sarg_test_c ${TEST_C_SRC})
add_executable(sarg_sample_get_c "sample/sample_get.c")
add_executable(sarg_sample_cb_c  "sample/sample_cb.c")
add_executable(sarg_sample_get_cpp "sample/sample_get.cpp")
#add_executable(sarg_sample_cb_cpp  "sample/sample_cb.cpp")

target_link_libraries(sarg_sample_get_c m)
target_link_libraries(sarg_sample_cb_c m)
target_link_libraries(sarg_sample_get_cpp m)
#target_link_libraries(sarg_sample_cb_cpp m)
