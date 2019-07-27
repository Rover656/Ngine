# Fetch source files
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h")

# Versioning
set(NGINE_VERSION_MAJOR 0)
set(NGINE_VERSION_MINOR 1)
set(NGINE_VERSION_PATCH 0)

# Product information (Some for UWP)
set(PROJECT_NAME Ngine)
set(SHORT_NAME ${PROJECT_NAME})
set(PACKAGE_GUID "0fa9fe36-15ee-44bb-a9b9-62d55c4ea028")
set(PUBLISHER "NerdThings")
set(PUBLISHER_DISPLAY_NAME "NerdThings")

if (${PLATFORM} MATCHES "Desktop")
    if (${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
        message(FATAL_ERROR "Cannot build for Desktop under UWP. Use -DPLATFORM=UWP instead.")
    endif()

    if (MSVC)
        # Generate windows product info
        include(generate_product_version)
        generate_product_version(
                VersionFilesOutputVariable
                NAME ${PROJECT_NAME}
                VERSION_MAJOR ${NGINE_VERSION_MAJOR}
                VERSION_MINOR ${NGINE_VERSION_MINOR}
                VERSION_PATCH ${NGINE_VERSION_PATCH}
                VERSION_REVISION 0
                COMPANY_NAME ${PUBLISHER_DISPLAY_NAME}
        )
    endif()

    if (${OPENGL_VERSION} MATCHES "3.3")
        # We need OpenGL
        find_package(OpenGL REQUIRED)

        # Ngine with native OpenGL
        if (${BUILD_SHARED})
            if (MSVC)
                add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${VersionFilesOutputVariable} ${PROJECT_SOURCE_DIR}/third-party/glad/src/glad.c)
            else()
                add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ../third-party/glad/src/glad.c)
            endif()
            target_compile_definitions(${PROJECT_NAME} PRIVATE NGINE_EXPORTS=1)
        else()
            add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES} ${PROJECT_SOURCE_DIR}/third-party/glad/src/glad.c)
        endif()

        # Link OpenGL
        target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

        # Link glfw
        target_link_libraries(${PROJECT_NAME} glfw)
        target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/third-party/glfw/include)

        # Link glad
        target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/third-party/glad/include)

        # Link Box2D
        target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/third-party/Box2D-cmake/Box2D")
        target_link_libraries(${PROJECT_NAME} tobanteGaming::Box2D)

        # Defines
        target_compile_definitions(${PROJECT_NAME} PUBLIC GRAPHICS_OPENGL33=1 PLATFORM_DESKTOP=1)
        target_compile_definitions(${PROJECT_NAME} PRIVATE NGINE_BUILD=1) # Used by ngine.h
    elseif(${OPENGL_VERSION} MATCHES "ES2")
        # Ngine with ANGLE
        if (${BUILD_SHARED})
            if (MSVC)
                add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES} ${VersionFilesOutputVariable})
            else()
                add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
            endif()
            target_compile_definitions(${PROJECT_NAME} PRIVATE NGINE_EXPORTS=1)
        else()
            add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
        endif()

        # Link glfw
        target_link_libraries(${PROJECT_NAME} glfw)
        target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/third-party/glfw/include)

        # Link ANGLE
        target_link_libraries(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libEGL.lib ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libGLESv2.lib)
        target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/third-party/ANGLE)

        # Link Box2D
        target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/third-party/Box2D-cmake/Box2D")
        target_link_libraries(${PROJECT_NAME} tobanteGaming::Box2D)

        # Defines
        target_compile_definitions(${PROJECT_NAME} PUBLIC GRAPHICS_OPENGLES2=1 PLATFORM_DESKTOP=1)
        target_compile_definitions(${PROJECT_NAME} PRIVATE NGINE_BUILD=1) # Used by ngine.h

        # Copy dependant dlls
        add_custom_command(TARGET Ngine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libEGL.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)

        add_custom_command(TARGET Ngine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libGLESv2.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)

        add_custom_command(TARGET Ngine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/d3dcompiler_47.dll
                $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endif()
elseif(${PLATFORM} MATCHES "UWP")
    # THIS SHOULD BE BUILT WITH A PROVIDED SCRIPT, NOT WITH CMAKE!! PROCEED AT YOUR OWN RISK
    if(${OPENGL_VERSION} MATCHES "3.3")
        message(WARNING "The selected OpenGL version is incompatible. Using GLES2 instead.")
    endif()
    if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
        message(FATAL_ERROR "You must build UWP with -DCMAKE_SYSTEM_NAME=WindowsStore")
    endif()
    if (NOT ${CMAKE_SYSTEM_VERSION} STREQUAL "10.0")
        message(FATAL_ERROR "You must build UWP with -DCMAKE_SYSTEM_VERSION=10.0")
    endif()

    # Source groups
    source_group("Source Files" FILES ${SOURCE_FILES})
    source_group("Header Files" FILES ${HEADER_FILES})

    # Add executable
    if (${BUILD_SHARED})
        add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${HEADER_FILES})
        target_compile_definitions(${PROJECT_NAME} PRIVATE NGINE_EXPORTS=1)
    else()
        add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})
    endif()

    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_WINRT_COMPONENT TRUE) # TODO: Is this required?

    # Defined
    target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHICS_OPENGLES2=1 PLATFORM_UWP=1)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NGINE_BUILD=1) # Used by ngine.h

    # Link Box2D
    target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/third-party/Box2D-cmake/Box2D")
    target_link_libraries(${PROJECT_NAME} tobanteGaming::Box2D)

    # Use ANGLE
    target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/third-party/ANGLE)
    configure_file(${PROJECT_SOURCE_DIR}/cmake/UWP/packages.config.in
            ${CMAKE_BINARY_DIR}/packages.config COPYONLY)

    # Configure the script ready for the project mod
    configure_file(${PROJECT_SOURCE_DIR}/cmake/UWP/UWPConfigureProject.ps1.in
            ${CMAKE_BINARY_DIR}/UWPConfigureProject.ps1)

    # Restore NuGet
    execute_process(COMMAND ${PROJECT_SOURCE_DIR}/redist/nuget.exe restore -SolutionDirectory ${CMAKE_BINARY_DIR})
endif()