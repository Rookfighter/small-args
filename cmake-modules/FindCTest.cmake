# FindCTest.cmake
#
# Created on: 07 May 2017
#     Author: Fabian Meyer
#    License: MIT
#
# provides:
#
# CTEST_FOUND
# CTEST_INCLUDE_DIR

find_path(CTEST_INCLUDE_DIR
    NAMES ctest.h
    HINTS "${CTEST_ROOT}"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CTest
    DEFAULT_MSG
    CTEST_INCLUDE_DIR
)
