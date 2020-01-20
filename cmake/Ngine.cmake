# TODO: Review needed, determine how to handle shared vs static Ngine linking

# General functions
include (CMakeParseArguments)
set(NGINE_MODULE_DIR ${CMAKE_CURRENT_LIST_DIR})

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

# TODO: Revisit this
function(ngine_add_library target shared)
    # Add lib
    if (${shared})
        add_library(${target} SHARED ${ARGN})
    else()
        add_library(${target} ${ARGN})
    endif()

    # Link
    __ngine_link_ngine(${target})

    # UWP additions
    __ngine_post_uwp_additions(${target})
endfunction()

function(ngine_add_game)
    # Get parameters
    set (options)
    set (oneValueArgs
            # Game info
            NAME # Game name
            SHORT_NAME # Shortened game name
            TARGET_NAME # Target name
            DESCRIPTION # Game description, otherwise it will be set to short name

            # Content
            CONTENT_DIR # Content directory

            # Version info
            VERSION_MAJOR
            VERSION_MINOR
            VERSION_PATCH

            # Package info
            PUBLISHER_NAME # Publisher name
            COPYRIGHT # Package copyright notice

            # Windows specifics
            WIN_ICON # Executable icon (Windows)

            UWP_PACKAGE_NAME # Ex 00000DeveloperName.AppName
            UWP_PACKAGE_GUID
            UWP_PUBLISHER # Publisher ID or name if unknown, CN=...

            # UWP Asset names
            UWP_STORE_LOGO_NAME # StoreLogo.png
            UWP_SPLASH_SCREEN_NAME # SplashScreen.png
            UWP_310x150_WIDE_NAME # Wide310x150Logo.png
            UWP_44x44_SQUARE_NAME # Square44x44Logo.png
            UWP_150x150_SQUARE_NAME # Square150x150Logo.png

            # UWP Store association resources
            UWP_PACKAGE_STORE_ASSOCIATION_PATH # If set, included in project root
            UWP_PACKAGE_STORE_KEY_PATH # If set, included in project root
            )
    set (multiValueArgs
            # Standard files
            SOURCE_FILES
            HEADER_FILES
            RESOURCE_FILES

            # Platform specifics
            DESKTOP_SOURCE_FILES
            DESKTOP_HEADER_FILES
            DESKTOP_RESOURCE_FILES

            UWP_SOURCE_FILES
            UWP_HEADER_FILES
            UWP_RESOURCE_FILES
            )
    cmake_parse_arguments(GAME "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # TODO: UWP Capabilities

    # Argument validation
    if (NOT GAME_NAME OR "${GAME_NAME}" STREQUAL "")
        set(GAME_NAME "Made with Ngine")
    endif()

    if (NOT GAME_SHORT_NAME OR "${GAME_SHORT_NAME}" STREQUAL "")
        set(GAME_SHORT_NAME ${GAME_NAME})
    endif()

    if (NOT GAME_TARGET_NAME OR "${GAME_TARGET_NAME}" STREQUAL "")
        message(FATAL_ERROR "[Ngine] Target name cannot be blank")
    endif()

    if (NOT GAME_DESCRIPTION OR "${GAME_DESCRIPTION}" STREQUAL "")
        set(GAME_DESCRIPTION ${GAME_SHORT_NAME})
    endif()

    if (NOT GAME_CONTENT_DIR OR "${GAME_CONTENT_DIR}" STREQUAL "")
        set(GAME_CONTENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/content")
    endif()

    if (NOT GAME_VERSION_MAJOR OR "${GAME_VERSION_MAJOR}" STREQUAL "")
        set(GAME_VERSION_MAJOR "1")
    endif()

    if (NOT GAME_VERSION_MINOR OR "${GAME_VERSION_MINOR}" STREQUAL "")
        set(GAME_VERSION_MINOR "0")
    endif()

    if (NOT GAME_VERSION_PATCH OR "${GAME_VERSION_PATCH}" STREQUAL "")
        set(GAME_VERSION_PATCH "0")
    endif()

    if (NOT GAME_PUBLISHER_NAME OR "${GAME_PUBLISHER_NAME}" STREQUAL "")
        message(FATAL_ERROR "[Ngine] Must set the publisher name")
    endif()

    if (NOT GAME_COPYRIGHT OR "${GAME_COPYRIGHT}" STREQUAL "")
        string (TIMESTAMP CURRENT_YEAR "%Y")
        set(GAME_COPYRIGHT "(C) ${GAME_PUBLISHER_NAME} ${CURRENT_YEAR}")
    endif()

    if (NOT GAME_WIN_ICON OR "${GAME_WIN_ICON}" STREQUAL "")
        set(GAME_HAS_ICON "//")
    endif()

    if (NOT GAME_UWP_PACKAGE_NAME OR "${GAME_UWP_PACKAGE_NAME}" STREQUAL "")
        set(GAME_UWP_PACKAGE_NAME ${GAME_NAME})
    endif()

    if (NOT GAME_UWP_PACKAGE_GUID OR "${GAME_UWP_PACKAGE_GUID}" STREQUAL "")
        if (${PLATFORM} MATCHES "UWP")
            message(FATAL_ERROR "[Ngine] Must set the UWP package GUID")
        else()
            message("[Ngine] You must set the UWP package GUID if you wish to target UWP.")
        endif()
    endif()

    if (NOT GAME_UWP_PUBLISHER OR "${GAME_UWP_PUBLISHER}" STREQUAL "")
        if (${PLATFORM} MATCHES "UWP")
            message(WARNING "[Ngine] Setting publisher parameter to publisher display name.")
        endif()
        set(GAME_UWP_PUBLISHER ${GAME_PUBLISHER_NAME})
    endif()

    if (NOT GAME_UWP_STORE_LOGO_NAME OR "${GAME_UWP_STORE_LOGO_NAME}" STREQUAL "")
        set(GAME_UWP_STORE_LOGO_NAME "StoreLogo.png")
    endif()

    if (NOT GAME_UWP_SPLASH_SCREEN_NAME OR "${GAME_UWP_SPLASH_SCREEN_NAME}" STREQUAL "")
        set(GAME_UWP_SPLASH_SCREEN_NAME "SplashScreen.png")
    endif()

    if (NOT GAME_UWP_310x150_WIDE_NAME OR "${GAME_UWP_310x150_WIDE_NAME}" STREQUAL "")
        set(GAME_UWP_310x150_WIDE_NAME "Wide310x150Logo.png")
    endif()

    if (NOT GAME_UWP_44x44_SQUARE_NAME OR "${GAME_UWP_44x44_SQUARE_NAME}" STREQUAL "")
        set(GAME_UWP_44x44_SQUARE_NAME "Square44x44Logo.png")
    endif()

    if (NOT GAME_UWP_150x150_SQUARE_NAME OR "${GAME_UWP_150x150_SQUARE_NAME}" STREQUAL "")
        set(GAME_UWP_150x150_SQUARE_NAME "Square150x150Logo.png")
    endif()

    # Get content files
    file(GLOB_RECURSE GAME_CONTENT_FILES "${GAME_CONTENT_DIR}/*")

    if(${PLATFORM} MATCHES "UWP")
        # Setup UWP manifest
        if (${PLATFORM} MATCHES "UWP")
            # Configure manifest
            configure_file(
                    ${NGINE_MODULE_DIR}/UWP/Package.appxmanifest.in
                    ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest
                    @ONLY)

            set(GAME_RESOURCE_FILES ${GAME_RESOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest)
        endif()

        # UWP Store association
        if (NOT "${GAME_UWP_PACKAGE_STORE_ASSOCIATION_PATH}" STREQUAL "")
            file (COPY ${GAME_UWP_PACKAGE_STORE_ASSOCIATION_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
            get_filename_component(KEY_NAME ${GAME_UWP_PACKAGE_STORE_KEY_PATH} NAME)
            set(ADDITIONAL_FILES ${ADDITIONAL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/Package.StoreAssociation.xml)
        endif()

        if (NOT "${GAME_UWP_PACKAGE_STORE_KEY_PATH}" STREQUAL "")
            file (COPY ${GAME_UWP_PACKAGE_STORE_KEY_PATH} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
            set(ADDITIONAL_FILES ${ADDITIONAL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${KEY_NAME})
        endif()

        # Add UWP sources
        set(GAME_SOURCE_FILES ${GAME_SOURCE_FILES} ${GAME_UWP_SOURCE_FILES})
        set(GAME_HEADER_FILES ${GAME_HEADER_FILES} ${GAME_UWP_HEADER_FILES})
        set(GAME_RESOURCE_FILES ${GAME_RESOURCE_FILES} ${GAME_UWP_RESOURCE_FILES})
    endif()

    # Windows versioning info
    if(${PLATFORM} MATCHES "Desktop")
        if (MSVC)
            # Generate product info
            configure_file(
                    ${NGINE_MODULE_DIR}/WIN/VersionInfo.in
                    ${CMAKE_CURRENT_BINARY_DIR}/VersionInfo.h
                    @ONLY)
            configure_file(
                    ${NGINE_MODULE_DIR}/WIN/VersionResource.rc
                    ${CMAKE_CURRENT_BINARY_DIR}/VersionResource.rc
                    @ONLY)
            set(ADDITIONAL_FILES ${ADDITIONAL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/VersionInfo.h ${CMAKE_CURRENT_BINARY_DIR}/VersionResource.rc)
        endif()

        # Add desktop sources
        set(GAME_SOURCE_FILES ${GAME_SOURCE_FILES} ${GAME_DESKTOP_SOURCE_FILES})
        set(GAME_HEADER_FILES ${GAME_HEADER_FILES} ${GAME_DESKTOP_HEADER_FILES})
        set(GAME_RESOURCE_FILES ${GAME_RESOURCE_FILES} ${GAME_DESKTOP_RESOURCE_FILES})
    endif()

    # Configure game
    add_executable(${GAME_TARGET_NAME} WIN32 ${GAME_SOURCE_FILES} ${GAME_HEADER_FILES} ${GAME_CONTENT_FILES} ${GAME_RESOURCE_FILES} ${ADDITIONAL_FILES})

    # Visual Studio Source Groups
    if(MSVC)
        source_group(TREE ${GAME_CONTENT_DIR} PREFIX "\\Content" FILES ${GAME_CONTENT_FILES})
        source_group("Assets" FILES ${GAME_RESOURCE_FILES})
    endif()

    # UWP Specifics
    if (${PLATFORM} MATCHES "UWP")
        # Mark as WinRT Component
        set_property(TARGET ${GAME_TARGET_NAME} PROPERTY VS_WINRT_COMPONENT TRUE)

        # Link special libraries
        target_link_libraries(${GAME_TARGET_NAME} WindowsApp)

        # No warnings
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()

    # Link Ngine
    __ngine_link_ngine(${GAME_TARGET_NAME})

    # Get content dir name
    get_filename_component(CONTENT_DIR_NAME ${GAME_CONTENT_DIR} NAME)

    ### Feature Flags

    # Experimental renderer
    if (${FEATURE_EXPERIMENTAL_RENDERER})
        target_compile_definitions(${GAME_TARGET_NAME} PRIVATE USE_EXPERIMENTAL_RENDERER=1)
    endif()

    ### End Feature Flags

    # Include content
    if (${PLATFORM} MATCHES "Desktop")
        add_custom_command(TARGET ${GAME_TARGET_NAME} PRE_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${GAME_CONTENT_DIR}
                $<TARGET_FILE_DIR:${GAME_TARGET_NAME}>/${CONTENT_DIR_NAME})
    elseif(${PLATFORM} MATCHES "UWP")
        # Mark for deploy
        set_property(SOURCE ${GAME_CONTENT_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)

        # Setup deployment directories
        foreach(CONTENT_FILE ${GAME_CONTENT_FILES})
            # Get filename
            get_filename_component(CONTENT_FILENAME ${CONTENT_FILE} NAME)

            # Remove path
            string(REPLACE "${GAME_CONTENT_DIR}/" "" CONTENT_FILE_DEPLOYLOC ${CONTENT_FILE})

            # Remove filename
            string(REPLACE "${CONTENT_FILENAME}" "" CONTENT_FILE_DEPLOYLOC ${CONTENT_FILE_DEPLOYLOC})

            # Add content dir name to start
            set(CONTENT_FILE_DEPLOYLOC ${CONTENT_DIR_NAME}/${CONTENT_FILE_DEPLOYLOC})

            # Remove trailing slash
            string(REGEX REPLACE "\\/$" "" CONTENT_FILE_DEPLOYLOC ${CONTENT_FILE_DEPLOYLOC})

            set_property(SOURCE ${CONTENT_FILE} PROPERTY VS_DEPLOYMENT_LOCATION ${CONTENT_FILE_DEPLOYLOC})
        endforeach()

        # Mark as deployable
        foreach(RESOURCE_FILE ${GAME_RESOURCE_FILES})
            # TODO: Support subdirectories
            set_property(SOURCE ${RESOURCE_FILE} PROPERTY VS_DEPLOYMENT_CONTENT 1)
            set_property(SOURCE ${RESOURCE_FILE} PROPERTY VS_DEPLOYMENT_LOCATION "Assets")
        endforeach()
    endif()
endfunction()

# Link Ngine and schedule copying
function(__ngine_link_ngine GAME_TARGET_NAME)
    # Link
    target_include_directories(${GAME_TARGET_NAME} PRIVATE ${Ngine_SOURCE_DIR}/include)
    target_link_libraries(${GAME_TARGET_NAME} Ngine)

    # Copy shared lib
    if (${BUILD_SHARED})
        # Mark as shared
        target_compile_definitions(${GAME_TARGET_NAME} PRIVATE NGINE_SHARED=1)

        # Copy, attached to Ngine target so that it will always happen
        if(${PLATFORM} MATCHES "Desktop")
            add_custom_command(TARGET ${GAME_TARGET_NAME}
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:Ngine>
                    $<TARGET_FILE_DIR:${GAME_TARGET_NAME}>)
        endif()
    endif()

    if (${OPENGL_VERSION} MATCHES "3.3")
        target_compile_definitions(${GAME_TARGET_NAME} PRIVATE GRAPHICS_OPENGL33=1)
    elseif (${OPENGL_VERSION} MATCHES "2.1")
        target_compile_definitions(${GAME_TARGET_NAME} PRIVATE GRAPHICS_OPENGL21=1)
    elseif(${OPENGL_VERSION} MATCHES "ES2")
        target_compile_definitions(${GAME_TARGET_NAME} PRIVATE GRAPHICS_OPENGLES2=1)

        if(${PLATFORM} MATCHES "Desktop")
            add_custom_command(TARGET ${GAME_TARGET_NAME}
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${NGINE_SOURCE_DIR}/../third-party/ANGLE/lib/x86/libEGL.dll
                    $<TARGET_FILE_DIR:${GAME_TARGET_NAME}>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${NGINE_SOURCE_DIR}/../third-party/ANGLE/lib/x86/libGLESv2.dll
                    $<TARGET_FILE_DIR:${GAME_TARGET_NAME}>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${NGINE_SOURCE_DIR}/../third-party/ANGLE/lib/x86/d3dcompiler_47.dll
                    $<TARGET_FILE_DIR:${GAME_TARGET_NAME}>)
        endif()
    endif()
endfunction()