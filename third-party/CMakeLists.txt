# Third Party library management

# Add libraries
add_subdirectory("Box2D-cmake")
if (${PLATFORM} MATCHES "Desktop")
    # Set GLFW Properties
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build glfw as a shared library")
    add_subdirectory(glfw)

    # Output directories
    set_target_properties(glfw PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/glfw"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/glfw"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/glfw")

    if (MSVC)
        set_target_properties(glfw PROPERTIES
                LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/glfw"
                LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/glfw")
    endif(MSVC)
endif()