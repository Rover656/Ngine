# General functions

function(ngine_add_executable target)
    # Add exe
    add_executable(${target} WIN32 ${ARGN})

    # UWP Specifics
    if (${PLATFORM} MATCHES "UWP")
        # Mark as WinRT Component
        set_property(TARGET ${target} PROPERTY VS_WINRT_COMPONENT TRUE) # IDK if we really need this tho

        # Link special libraries
        target_link_libraries(${target} WindowsApp)
    endif()
endfunction()

function(ngine_add_library target shared)
    # Add lib
    if (${shared})
        add_library(${target} SHARED ${ARGN})
    else()
        add_library(${target} ${ARGN})
    endif()

    # UWP Specifics
    if (${PLATFORM} MATCHES "UWP")
        # Mark as WinRT Component
        set_property(TARGET ${target} PROPERTY VS_WINRT_COMPONENT TRUE) # IDK if we really need this tho

        # Link special libraries
        target_link_libraries(${target} WindowsApp)
    endif()
endfunction()

function(ngine_configure_uwp_executable manifest PROJECT_NAME PACKAGE_GUID PUBLISHER PUBLISHER_DISPLAY_NAME VERSION_MAJOR VERSION_MINOR VERSION_PATCH)
    if (${PLATFORM} MATCHES "UWP")
        # Configure manifest
        configure_file(
                ${manifest}
                ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest
                @ONLY)
    endif()
endfunction()

# Check all config is okay
function(ngine_check_config)
    if (${PLATFORM} MATCHES "Desktop")
        if (${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
            message(FATAL_ERROR "Cannot build Desktop for UWP. Use UWP platform instead.")
        endif()
    elseif(${PLATFORM} MATCHES "UWP")
        if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
            message(FATAL_ERROR "You must build UWP with -DCMAKE_SYSTEM_NAME=WindowsStore")
        endif()
        if (NOT ${CMAKE_SYSTEM_VERSION} STREQUAL "10.0")
            message(FATAL_ERROR "You must build UWP with -DCMAKE_SYSTEM_VERSION=10.0")
        endif()
    endif()
endfunction()

# Link Ngine and schedule copying
function(ngine_link_ngine target)
    get_target_property(NGINE_SOURCE_DIR Ngine SOURCE_DIR)

    # Link
    target_include_directories(${target} PRIVATE ${NGINE_SOURCE_DIR})
    target_link_libraries(${target} Ngine)

    # Copy shared lib
    if (${BUILD_SHARED})
        # Mark as shared
        target_compile_definitions(${target} PRIVATE NGINE_SHARED=1)

        # Copy, attached to Ngine target so that it will always happen
        if(${PLATFORM} MATCHES "Desktop")
            add_custom_command(OUTPUT ${target}copyNgine
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:Ngine>
                    $<TARGET_FILE_DIR:${target}>)
        endif()
    endif()

    if (${OPENGL_VERSION} MATCHES "3.3")
        target_compile_definitions(${target} PRIVATE GRAPHICS_OPENGL33=1)
    elseif(${OPENGL_VERSION} MATCHES "ES2")
        target_compile_definitions(${target} PRIVATE GRAPHICS_OPENGLES2=1)

        if(${PLATFORM} MATCHES "Desktop")
            add_custom_command(OUTPUT ${target}copyES2
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

    if (${PLATFORM} MATCHES "Desktop")
        target_compile_definitions(${target} PRIVATE PLATFORM_DESKTOP=1)
    elseif(${PLATFORM} MATCHES "UWP")
        target_compile_definitions(${target} PRIVATE PLATFORM_UWP=1)
    endif()
endfunction()

# Content Management

function(ngine_add_content target root_dir content_dir)
    if (${PLATFORM} MATCHES "Desktop")
        add_custom_command(TARGET ${target} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${root_dir}/${content_dir}
                $<TARGET_FILE_DIR:NgineTest>/${content_dir})
    elseif(${PLATFORM} MATCHES "UWP")
        # Get all files
        file(GLOB_RECURSE content_files "${root_dir}/${content_dir}/*")

        # Mark for deploy
        set_property(SOURCE ${content_files} PROPERTY VS_DEPLOYMENT_CONTENT 1)

        # Setup deployment directories
        foreach(CONTENT_FILE ${content_files})
            # Get filename
            get_filename_component(CONTENT_FILENAME ${CONTENT_FILE} NAME)

            # Remove initial path
            string(REPLACE "${root_dir}/" "" CONTENT_FILE_DEPLOYLOC ${CONTENT_FILE})

            # Remove filename
            string(REPLACE "/${CONTENT_FILENAME}" "" CONTENT_FILE_DEPLOYLOC ${CONTENT_FILE_DEPLOYLOC})

            message(${CONTENT_FILE})
            message(${CONTENT_FILE_DEPLOYLOC})
            set_property(SOURCE ${CONTENT_FILE} PROPERTY VS_DEPLOYMENT_LOCATION ${CONTENT_FILE_DEPLOYLOC})
        endforeach()

        # Resource files
        if (ARGN)
            # Mark as deployable
            set_property(SOURCE ${RESOURCE_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)
            set_property(SOURCE ${RESOURCE_FILES} PROPERTY VS_DEPLOYMENT_LOCATION "Assets")
        endif()
    endif()
endfunction()