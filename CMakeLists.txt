# Mark minimum cmake version
cmake_minimum_required(VERSION 3.12)

# Setup project
project(Ngine
        VERSION 1.0.0
        DESCRIPTION "A 2D Game Engine")

# Add a suffix
set(PROJECT_VERSION_SUFFIX "dev")
if (NOT ${PROJECT_VERSION_SUFFIX} STREQUAL "")
    set(PROJECT_VERSION ${PROJECT_VERSION}-${PROJECT_VERSION_SUFFIX})
    set(Ngine_VERSION ${PROJECT_VERSION})
endif()

# Include modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(EnumOption)

# Options
option(BUILD_TEST "Build the test program." ON)
option(BUILD_DOCS "Build documentation" ON)

option(BUILD_SHARED_LIBS "Build Ngine as a shared library. This will block building of a static library." OFF)

enum_option(PLATFORM "Desktop;UWP" "The platform to build for.")
enum_option(RENDERER "GL33;GL21;GLES2" "The preferred renderer/graphics API.")

# Setup project (if we are the main project)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    # C++17
    set(CMAKE_CXX_STANDARD 17)

    # -fPIC
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# Project details
message(STATUS "Building Ngine V${PROJECT_VERSION}")

### Feature Flags

option(FEATURE_EXPERIMENTAL_RENDERER "Enable the experimental renderer." OFF)

### End Feature Flags

# Option validation
if(${PLATFORM} MATCHES "Desktop")

elseif (${PLATFORM} MATCHES "UWP")
    if (NOT ${RENDERER} MATCHES "ES2")
        message(WARNING "UWP does not support OpenGL Version ${OPENGL_VERSION}")
        set(RENDERER "GLES2")
    endif()

    if (${RENDERER} MATCHES "ES2")
        if (NOT MSVC)
            message("Cannot use OpenGL ES2 on non-windows.")
            set(RENDERER "GL33")
        endif()
    endif()
endif()

# Broadcast renderer
message("Building for the \"${PLATFORM}\" platform with the \"${RENDERER}\" renderer.")

# Add sub directories
add_subdirectory(third-party)
add_subdirectory(src)

# Extra subdirectories if building as main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if (${BUILD_DOCS})
        add_subdirectory(docs)
    endif()

    if (${BUILD_TEST})
        add_subdirectory(test)
    endif()
endif()