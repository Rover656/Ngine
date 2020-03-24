# Fetch source files
file(GLOB_RECURSE _NGINE_SRC "${Ngine_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE _NGINE_PRIV_HEADERS "${Ngine_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE _NGINE_HEADERS "${Ngine_SOURCE_DIR}/include/*.hpp")

# Setup extras
set(_NGINE_PUBLIC_INCLUDE_DIRS "")
set(_NGINE_PRIVATE_INCLUDE_DIRS "")
set(_NGINE_LIBRARIES "")

# Configure config
configure_file(${PROJECT_SOURCE_DIR}/include/ngine/config.hpp.in ${Ngine_BINARY_DIR}/include/ngine/config.hpp)

# GLAD
if (NOT PLATFORM_UWP)
    if (NGINE_ENABLE_OPENGL OR NGINE_ENABLE_OPENGLES)
        set(_NGINE_SRC ${_NGINE_SRC} ${Ngine_SOURCE_DIR}/vendor/glad/src/glad.c)
        set(_NGINE_PRIVATE_INCLUDE_DIRS ${_NGINE_PRIVATE_INCLUDE_DIRS} ${Ngine_SOURCE_DIR}/vendor/glad/include)
    endif()
endif()

# GLFW
if (PLATFORM_DESKTOP)
    set(_NGINE_PRIVATE_INCLUDE_DIRS ${_NGINE_PRIVATE_INCLUDE_DIRS} ${Ngine_SOURCE_DIR}/vendor/glfw/include)
    set(_NGINE_LIBRARIES ${_NGINE_LIBRARIES} glfw)
endif()

# Setup projects
if (BUILD_NGINE_STATIC)
    # Add library
    add_library(Ngine_static ${_NGINE_SRC} ${_NGINE_PRIV_HEADERS} ${_NGINE_HEADERS})

    # Setup language features
    target_compile_features(Ngine_static PUBLIC cxx_std_17)

    # Include directories and linkages
    target_link_libraries(Ngine_static
            PRIVATE
            box2d
            ${_NGINE_LIBRARIES})
    target_include_directories(Ngine_static
            PUBLIC
            "${Ngine_SOURCE_DIR}/include"
            "${Ngine_BINARY_DIR}/include" # Add the configured config.hpp
            "${Ngine_SOURCE_DIR}/vendor/miniaudio"
            ${_NGINE_PUBLIC_INCLUDE_DIRS}
            PRIVATE
            "${Ngine_SOURCE_DIR}/src"
            "${Ngine_SOURCE_DIR}/vendor/stb"
            ${_NGINE_PRIVATE_INCLUDE_DIRS})
endif()

if (BUILD_NGINE_SHARED)
    # Add library
    add_library(Ngine SHARED ${_NGINE_SRC} ${_NGINE_PRIV_HEADERS} ${_NGINE_HEADERS})

    # Mark for exporting
    target_compile_definitions(Ngine PRIVATE NGINE_EXPORTS=1)

    # Setup language features
    target_compile_features(Ngine_static PUBLIC cxx_std_17)

    # Include directories and linkages
    target_link_libraries(Ngine_static
            PRIVATE
            box2d
            ${_NGINE_LIBRARIES})
    target_include_directories(Ngine_static
            PUBLIC
            "${Ngine_SOURCE_DIR}/include"
            "${Ngine_BINARY_DIR}/include" # Add the configured config.hpp
            "${Ngine_SOURCE_DIR}/vendor/miniaudio"
            ${_NGINE_PUBLIC_INCLUDE_DIRS}
            PRIVATE
            "${Ngine_SOURCE_DIR}/src"
            "${Ngine_SOURCE_DIR}/vendor/stb"
            ${_NGINE_PRIVATE_INCLUDE_DIRS})
endif()

# TODO: Doxygen and UWP support