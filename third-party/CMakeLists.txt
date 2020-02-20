# Third Party library management

# Add libraries
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE INTERNAL "Build the Box2D unit tests")
set(BOX2D_BUILD_TESTBED OFF CACHE INTERNAL "Build the Box2D testbed")
add_subdirectory("box2d")

if (${PLATFORM} MATCHES "Desktop")
    # Set GLFW Properties
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build glfw as a shared library")
    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(glfw)
endif()