cmake_minimum_required(VERSION 3.6)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(EnumOption)

# C++ 17
set(CMAKE_CXX_STANDARD 17)

# -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Project details
project("Ngine")

# Config

# Options
option(BUILD_TEST "Build the test program." ON)
option(BUILD_SHARED "Build as a shared library" ON)
enum_option(PLATFORM "Desktop;UWP" "Platform to build for.")
enum_option(OPENGL_VERSION "3.3;ES2" "Preferred OpenGL Version.")

message("Building for ${PLATFORM} with preferred OpenGL Version ${OPENGL_VERSION}.")

# Option enforcement
if (DEFINED BUILD_SHARED_LIBS)
    if (${BUILD_SHARED_LIBS})
        set(BUILD_SHARED ON CACHE INTERNAL "" FORCE)
    endif()
endif()

# Filesystem for Linux and MinGW
if (UNIX OR MINGW)
    target_link_libraries(NgineTest stdc++fs)
endif()

# Output directories
if (NOT ${PLATFORM} MATCHES "UWP") # TODO: See if this is actually necesary
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine")

    if (MSVC)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Ngine")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Ngine")
    endif(MSVC)
endif()

# Add sub directories
add_subdirectory(third-party)
add_subdirectory(src)

if (${BUILD_TEST})
	add_subdirectory(test)
endif()
