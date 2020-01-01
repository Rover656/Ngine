cmake_minimum_required(VERSION 3.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(EnumOption)

# C++ 17
set(CMAKE_CXX_STANDARD 17)

# -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Project details
project("Ngine")

# Options
option(BUILD_TEST "Build the test program." ON)
option(BUILD_SHARED "Build as a shared library" OFF)
enum_option(PLATFORM "Desktop;UWP" "Platform to build for.")
enum_option(OPENGL_VERSION "3.3;2.1;ES2" "Preferred OpenGL Version.")

# Feature flags
option(FEATURE_EXPERIMENTAL_RENDERER "Enable the experimental renderer." OFF)

# Force options
if(${PLATFORM} MATCHES "UWP")
    message("Forcing OpenGL Version to ES2")
    set(OPENGL_VERSION "ES2")
elseif (${PLATFORM} MATCHES "Desktop")
    if (${OPENGL_VERSION} MATCHES "ES2")
        if (NOT MSVC)
            message("Cannot use OpenGL ES2 on non-windows.")
            set(OPENGL_VERSION "3.3")
        endif()
    endif()
endif()

message("Building for ${PLATFORM} with preferred OpenGL Version ${OPENGL_VERSION}.")

# Option enforcement
if (DEFINED BUILD_SHARED_LIBS)
    if (${BUILD_SHARED_LIBS})
        set(BUILD_SHARED ON CACHE INTERNAL "" FORCE)
    endif()
endif()

# Output directories TODO: Revisit this structure
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

if (MSVC)
    #set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/lib")
    #set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/lib")
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release/bin")

    #set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug/lib")
    #set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug/lib")
    #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug/bin")
endif(MSVC)

# Add sub directories
add_subdirectory(third-party)
add_subdirectory(src)

if (${BUILD_TEST})
	add_subdirectory(test)
endif()
