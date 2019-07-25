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
        # Ngine with native OpenGL
        if (${BUILD_SHARED})
            if (MSVC)
                add_library(Ngine SHARED ${SOURCE_FILES} ${HEADER_FILES} ${VersionFilesOutputVariable} ../third-party/glad/src/glad.c)
            else()
                add_library(Ngine SHARED ${SOURCE_FILES} ${HEADER_FILES} ../third-party/glad/src/glad.c)
            endif()
        else()
            add_library(Ngine STATIC ${SOURCE_FILES} ${HEADER_FILES} ../third-party/glad/src/glad.c)
        endif()

        # Link OpenGL
        target_link_libraries(Ngine ${OPENGL_LIBRARIES})
        target_include_directories(Ngine PRIVATE ${OPENGL_INCLUDE_DIR})

        # Link glfw
        target_link_libraries(Ngine glfw)
        target_include_directories(Ngine PRIVATE ${PROJECT_SOURCE_DIR}/third-party/glfw/include)

        # Link glad
        target_include_directories(Ngine PRIVATE ${PROJECT_SOURCE_DIR}/third-party/glad/include)

        # Defines
        target_compile_definitions(Ngine PUBLIC GRAPHICS_OPENGL33=1 PLATFORM_DESKTOP=1)
    elseif(${OPENGL_VERSION} MATCHES "ES2")
        # Ngine with ANGLE
        if (${BUILD_SHARED})
            if (MSVC)
                add_library(Ngine SHARED ${SOURCE_FILES} ${HEADER_FILES} ${VersionFilesOutputVariable})
            else()
                add_library(Ngine SHARED ${SOURCE_FILES} ${HEADER_FILES})
            endif()
        else()
            add_library(Ngine STATIC ${SOURCE_FILES} ${HEADER_FILES})
        endif()

        # Link glfw
        target_link_libraries(Ngine glfw)
        target_include_directories(Ngine PRIVATE ../third-party/glfw/include)

        # Link ANGLE
        target_link_libraries(Ngine ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libEGL.lib ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libGLESv2.lib)
        target_include_directories(Ngine PRIVATE ${PROJECT_SOURCE_DIR}/third-party/ANGLE)

        # Defines
        target_compile_definitions(Ngine PUBLIC GRAPHICS_OPENGLES2=1 PLATFORM_DESKTOP=1)

        # Copy dependant dlls
        add_custom_command(TARGET Ngine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libEGL.dll
                $<TARGET_FILE_DIR:Ngine>)

        add_custom_command(TARGET Ngine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libGLESv2.dll
                $<TARGET_FILE_DIR:Ngine>)

        add_custom_command(TARGET Ngine POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/d3dcompiler_47.dll
                $<TARGET_FILE_DIR:Ngine>)
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

    # TODO: UWP Specific setup
endif()