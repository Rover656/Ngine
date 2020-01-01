# Fetch source files
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h" "*.hpp")

# Include Ngine cmake lib
include(Ngine)

# Versioning
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# Product information (Some for UWP)
set(PROJECT_NAME Ngine)
set(SHORT_NAME ${PROJECT_NAME})
set(PACKAGE_GUID "0fa9fe36-15ee-44bb-a9b9-62d55c4ea028")
set(PUBLISHER "NerdThings")
set(PUBLISHER_DISPLAY_NAME "NerdThings")

ngine_check_config()

if (${PLATFORM} MATCHES "Desktop" AND MSVC)
    # Generate windows product info
    include(generate_product_version)
    generate_product_version(
        EXTRA_FILES
        NAME ${PROJECT_NAME}
        VERSION_MAJOR ${NGINE_VERSION_MAJOR}
        VERSION_MINOR ${NGINE_VERSION_MINOR}
        VERSION_PATCH ${NGINE_VERSION_PATCH}
        VERSION_REVISION 0
        COMPANY_NAME ${PUBLISHER_DISPLAY_NAME})
endif()

if ((${OPENGL_VERSION} MATCHES "2.1" OR ${OPENGL_VERSION} MATCHES "3.3") AND ${PLATFORM} MATCHES "Desktop")
    # We need OpenGL
    find_package(OpenGL REQUIRED)

    # Add glad source
    set(EXTRA_FILES ${EXTRA_FILES} "../third-party/glad/src/glad.c")
endif()

# Add library
if (${BUILD_SHARED})
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${EXTRA_FILES})
else()
    add_library(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${EXTRA_FILES})
endif()

# UWP additions
__ngine_post_uwp_additions(${PROJECT_NAME})

# Enable exports
if (${BUILD_SHARED} AND NOT ${PLATFORM} MATCHES "UWP")
    target_compile_definitions(${PROJECT_NAME} PRIVATE NGINE_EXPORTS=1)
endif()

### Feature Flags

# Experimental renderer
if (${FEATURE_EXPERIMENTAL_RENDERER})
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_EXPERIMENTAL_RENDERER=1)
endif()

### End Feature Flags

# Add library dependancies
if (${PLATFORM} MATCHES "Desktop")
    if (${OPENGL_VERSION} MATCHES "3.3")
        # Link OpenGL
        target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

        # Mark OpenGL 3.3
        target_compile_definitions(${PROJECT_NAME} PUBLIC GRAPHICS_OPENGL33=1)
    elseif (${OPENGL_VERSION} MATCHES "2.1")
        # Link OpenGL
        target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

        # Mark OpenGL 3.3
        target_compile_definitions(${PROJECT_NAME} PUBLIC GRAPHICS_OPENGL21=1)
    elseif(${OPENGL_VERSION} MATCHES "ES2")
        # Link ANGLE
        target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libEGL.lib ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libGLESv2.lib)
        target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/third-party/ANGLE)

        # Mark OpenGLES2
        target_compile_definitions(${PROJECT_NAME} PUBLIC GRAPHICS_OPENGLES2=1)
    endif()

    # Link glfw
    target_link_libraries(${PROJECT_NAME} glfw)
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/third-party/glfw/include)

    # Link glad
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/third-party/glad/include)
elseif(${PLATFORM} MATCHES "UWP")
    # We check anyway, just in case...
    if (${OPENGL_VERSION} MATCHES "ES2")
        # Mark OpenGLES2
        target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHICS_OPENGLES2=1)
    endif()

    # Add ANGLE via NuGet
    configure_file(${PROJECT_SOURCE_DIR}/cmake/UWP/packages.config.in
            ${CMAKE_BINARY_DIR}/packages.config COPYONLY)
    execute_process(COMMAND ${PROJECT_SOURCE_DIR}/redist/nuget.exe restore -SolutionDirectory ${CMAKE_BINARY_DIR})

    # Link ANGLE
    target_link_libraries(Ngine ${CMAKE_BINARY_DIR}/packages/ANGLE.WindowsStore.2.1.13/build/native/ANGLE.WindowsStore.targets)
endif()

# Mark as build
target_compile_definitions(${PROJECT_NAME} PRIVATE NGINE_BUILD=1)

# Include stb libraries
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/third-party/stb)

# Link Box2D
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/third-party/Box2D-cmake/Box2D")
target_link_libraries(${PROJECT_NAME} tobanteGaming::Box2D)

# Miniaudio
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/third-party/miniaudio")

# Dependant static libs
if(${OPENGL_VERSION} MATCHES  "ES2")
    if(${PLATFORM} MATCHES "Desktop")
        add_custom_command(OUTPUT ${PROJECT_NAME}CopyES2
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${NGINE_SOURCE_DIR}/../third-party/ANGLE/lib/x86/libEGL.dll
                $<TARGET_FILE_DIR:${target}>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${NGINE_SOURCE_DIR}/../third-party/ANGLE/lib/x86/libGLESv2.dll
                $<TARGET_FILE_DIR:${target}>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${NGINE_SOURCE_DIR}/../third-party/ANGLE/lib/x86/d3dcompiler_47.dll
                $<TARGET_FILE_DIR:${target}>)
    endif()
endif()

# Visual Studio Source groups
if(MSVC)
    source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "\\Source Files" FILES ${SOURCE_FILES})
    source_group(TREE ${PROJECT_SOURCE_DIR}/src PREFIX "\\Header Files" FILES ${HEADER_FILES})
endif()
