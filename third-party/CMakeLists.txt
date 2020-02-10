# Third Party library management

# Add Chipmunk
set(BUILD_DEMOS OFF CACHE INTERNAL "Build the demo applications")
set(INSTALL_DEMOS OFF CACHE INTERNAL "Install the demo applications")
set(BUILD_SHARED OFF CACHE INTERNAL "Build and install the shared library")
set(BUILD_STATIC ON CACHE INTERNAL "Build as static library")
set(INSTALL_STATIC OFF CACHE INTERNAL "Install the static library")
add_subdirectory("Chipmunk2D")

# Remove debug messages etc.
target_compile_definitions(chipmunk_static PRIVATE NDEBUG)

if (${PLATFORM} MATCHES "Desktop")
    # Set GLFW Properties
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build glfw as a shared library")
    add_subdirectory(glfw)
endif()