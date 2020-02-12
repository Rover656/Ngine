# Third Party library management

# Add libraries
add_subdirectory("Box2D-cmake")
if (${PLATFORM} MATCHES "Desktop")
    # Set GLFW Properties
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build glfw as a shared library")
    add_subdirectory(glfw)
endif()