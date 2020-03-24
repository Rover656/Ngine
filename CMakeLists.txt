# Mark minimum cmake version
cmake_minimum_required(VERSION 3.12)

# Setup project
project(Ngine
        VERSION 1.0.0
        DESCRIPTION "A 2D Game Engine")

# Add a version suffix
set(PROJECT_VERSION_SUFFIX "rewrite")
if (NOT PROJECT_VERSION_SUFFIX STREQUAL "")
    set(PROJECT_VERSION ${PROJECT_VERSION}-${PROJECT_VERSION_SUFFIX})
    set(Ngine_VERSION ${PROJECT_VERSION})
endif ()

# Include modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(EnumOption)

# Options
option(BUILD_TEST "Build the test program." ON)
option(BUILD_DOCS "Build documentation." ON)
option(BUILD_EXAMPLES "Build examples." OFF)

option(BUILD_NGINE_STATIC "Build Ngine as a static library." ON)
option(BUILD_NGINE_SHARED "Build Ngine as a shared library." OFF)

enum_option(PLATFORM "Desktop;UWP" "The platform to build for.")

# Set platform variables for config.hpp
if (PLATFORM MATCHES "Desktop")
    set(PLATFORM_DESKTOP ON)
elseif(PLATFORM MATCHES "UWP")
    set(PLATFORM_UWP ON)
endif()

# Setup project (if we are the main project)
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # -fPIC
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

# Project details
message(STATUS "Building Ngine V${PROJECT_VERSION}")

### Feature Flags

option(NGINE_ENABLE_OPENGL "Enable the OpenGL Rendering API." ON)
option(NGINE_ENABLE_OPENGLES "Enable the OpenGL ES Rendering API. On Windows this will add ANGLE as a dependency." ON)

### End Feature Flags

# Option validation (basically disable any incompatible rendering pipelines.
if (PLATFORM MATCHES "Desktop")
    # TODO: Does GLES work on Mac/Linux. Do we need to block this?
elseif (PLATFORM MATCHES "UWP")
    set(NGINE_ENABLE_OPENGL OFF)
    set(NGINE_ENABLE_OPENGLES ON) # Force until DX support arrives.
endif ()

# Check options
if (PLATFORM MATCHES "Desktop")
    if (CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
        message(FATAL_ERROR "Cannot build Desktop for UWP. Use UWP platform instead.")
    endif ()
elseif (PLATFORM MATCHES "UWP")
    if (NOT CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
        message(FATAL_ERROR "You must build UWP with -DCMAKE_SYSTEM_NAME=WindowsStore")
    endif ()
    if (NOT CMAKE_SYSTEM_VERSION STREQUAL "10.0")
        message(FATAL_ERROR "You must build UWP with -DCMAKE_SYSTEM_VERSION=10.0")
    endif ()
endif ()

# Broadcast renderer
message(STATUS "Building Ngine for the \"${PLATFORM}\" platform.")
message(STATUS "Ngine OpenGL support is ${NGINE_ENABLE_OPENGL}")
message(STATUS "Ngine OpenGLES support is ${NGINE_ENABLE_OPENGLES}")

# Add sub directories
add_subdirectory(vendor)
add_subdirectory(src)

# Extra subdirectories if building as main project
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if (BUILD_DOCS)
        #add_subdirectory(docs)
    endif ()

    if (BUILD_TEST)
        add_subdirectory(test)
    endif ()
endif ()

if (BUILD_EXAMPLES)
    #add_subdirectory(examples)
endif()