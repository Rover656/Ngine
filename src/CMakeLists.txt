# Fetch source files
file(GLOB_RECURSE SOURCE_FILES "${Ngine_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${Ngine_SOURCE_DIR}/include/*.hpp")

# Generate versioning
if (${PLATFORM} MATCHES "Desktop" AND MSVC AND ${BUILD_SHARED_LIBS})
    # Generate windows product info
    include(generate_product_version)
    generate_product_version(
            EXTRA_FILES
            NAME Ngine
            VERSION_MAJOR ${Ngine_VERSION_MAJOR}
            VERSION_MINOR ${Ngine_VERSION_MINOR}
            VERSION_PATCH ${Ngine_VERSION_PATCH}
            VERSION_REVISION 0
            COMPANY_NAME ${PUBLISHER_DISPLAY_NAME})
endif ()

# Find dependencies
if (${PLATFORM} MATCHES "Desktop")
    if (${RENDERER} MATCHES "GL33" OR ${RENDERER} MATCHES "GL22")
        # Find OpenGL and add GLAD
        find_package(OpenGL REQUIRED)
        set(EXTRA_FILES ${EXTRA_FILES} "${Ngine_SOURCE_DIR}/third-party/glad/src/glad.c")

        # We need OpenGL
        set(_NGINE_LIBRARIES ${_NGINE_LIBRARIES} ${OPENGL_LIBRARIES})
        set(_NGINE_INCLUDE_DIRS ${_NGINE_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
    elseif (${RENDERER} MATCHES "GLES2")
        # We need ANGLE
        set(_NGINE_LIBRARIES ${_NGINE_LIBRARIES} ${Ngine_SOURCE_DIR}/third-party/ANGLE/lib/x86/libEGL.lib ${Ngine_SOURCE_DIR}/third-party/ANGLE/lib/x86/libGLESv2.lib)
        set(_NGINE_INCLUDE_DIRS ${_NGINE_INCLUDE_DIRS} ${Ngine_SOURCE_DIR}/third-party/ANGLE)

        # Copy static libs
        add_custom_command(OUTPUT NgineCopyES2
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${Ngine_SOURCE_DIR}/third-party/ANGLE/lib/x86/libEGL.dll
                $<TARGET_FILE_DIR:${target}>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${Ngine_SOURCE_DIR}/third-party/ANGLE/lib/x86/libGLESv2.dll
                $<TARGET_FILE_DIR:${target}>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${Ngine_SOURCE_DIR}/third-party/ANGLE/lib/x86/d3dcompiler_47.dll
                $<TARGET_FILE_DIR:${target}>)
    endif ()

    # Link glfw and glad
    set(_NGINE_LIBRARIES ${_NGINE_LIBRARIES} glfw)
    set(_NGINE_INCLUDE_DIRS ${_NGINE_INCLUDE_DIRS} ${Ngine_SOURCE_DIR}/third-party/glfw/include ${Ngine_SOURCE_DIR}/third-party/glad/include)
elseif (${PLATFORM} MATCHES "UWP")
    # Add ANGLE via NuGet
    configure_file(${Ngine_SOURCE_DIR}/cmake/UWP/packages.config.in
            ${CMAKE_BINARY_DIR}/packages.config COPYONLY)
    execute_process(COMMAND ${Ngine_SOURCE_DIR}/redist/nuget.exe restore -SolutionDirectory ${CMAKE_BINARY_DIR})

    # Link ANGLE
    set(_NGINE_LIBRARIES ${_NGINE_LIBRARIES} ${CMAKE_BINARY_DIR}/packages/ANGLE.WindowsStore.2.1.13/build/native/ANGLE.WindowsStore.targets)
endif ()

# Add stb, Box2D and Miniaudio
set(_NGINE_INCLUDE_DIRS
        ${_NGINE_INCLUDE_DIRS}
        "${Ngine_SOURCE_DIR}/third-party/stb"
        "${Ngine_SOURCE_DIR}/third-party/Box2D-cmake/Box2D"
        "${Ngine_SOURCE_DIR}/third-party/miniaudio")
set(_NGINE_LIBRARIES ${_NGINE_LIBRARIES} Box2D-cmake)

if (${BUILD_SHARED_LIBS})
    # Add library
    add_library(Ngine SHARED ${SOURCE_FILES} ${HEADER_FILES} ${EXTRA_FILES})

    # Mark for exporting
    target_compile_definitions(Ngine PRIVATE NGINE_EXPORTS=1)
else ()
    # Add library
    add_library(Ngine STATIC ${SOURCE_FILES} ${HEADER_FILES} ${EXTRA_FILES})
endif ()

# Depend on docs if enabled
if (${BUILD_DOCS})
    add_dependencies(Ngine Ngine_Docs)
endif ()

# UWP Specifics
if (${PLATFORM} MATCHES "UWP")
    # Mark as WinRT Component
    set_property(TARGET Ngine PROPERTY VS_WINRT_COMPONENT TRUE)

    # Link special libraries
    target_link_libraries(Ngine WindowsApp)

    # No warnings
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Include our headers
target_include_directories(Ngine PUBLIC ${Ngine_SOURCE_DIR}/include)

# Add our dependencies
target_link_libraries(Ngine ${_NGINE_LIBRARIES})
target_include_directories(Ngine PUBLIC ${_NGINE_INCLUDE_DIRS})

### Feature Flags

if (${FEATURE_EXPERIMENTAL_RENDERER})
    target_compile_definitions(Ngine PUBLIC USE_EXPERIMENTAL_RENDERER=1)
endif ()

### End Feature Flags

# Set Renderer Defines
if (${RENDERER} MATCHES "GL33")
    target_compile_definitions(Ngine PUBLIC GRAPHICS_OPENGL33=1)
elseif (${RENDERER} MATCHES "GL21")
    target_compile_definitions(Ngine PUBLIC GRAPHICS_OPENGL21=1)
elseif (${RENDERER} MATCHES "GLES2")
    target_compile_definitions(Ngine PUBLIC GRAPHICS_OPENGLES2=1)
endif ()

# Source Groups
source_group(TREE ${Ngine_SOURCE_DIR}/src PREFIX "\\Source Files" FILES ${SOURCE_FILES})
source_group(TREE ${Ngine_SOURCE_DIR}/include PREFIX "\\Header Files" FILES ${HEADER_FILES})