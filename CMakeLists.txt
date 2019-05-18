cmake_minimum_required(VERSION 3.6)

# C++ 17
set (CMAKE_CXX_STANDARD 17)

# Project details
project("Ngine")

# Options
option(BUILD_TEST "Build the test program." ON)

# TODO: Add static library build support

# Add sub directories
add_subdirectory(src)

if (${BUILD_TEST})
	add_subdirectory(test)
endif()