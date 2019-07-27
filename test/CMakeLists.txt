# Fetch source files
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

# Product information (Some for UWP)
set(PROJECT_NAME NgineTest)
set(SHORT_NAME ${PROJECT_NAME})
set(PACKAGE_GUID "62ab7c15-145e-4798-a843-a771f4ef8e9a")
set(PUBLISHER "NerdThings")
set(PUBLISHER_DISPLAY_NAME "NerdThings")

if (${PLATFORM} MATCHES "Desktop")
    if (${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
        message(FATAL_ERROR "Cannot build for Desktop under UWP. Use -DPLATFORM=UWP instead.")
    endif()

    # Add EXE
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})

    # Link Ngine
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}}/src)
    target_link_libraries(${PROJECT_NAME} Ngine)

    if (${BUILD_SHARED})
        # Use dll imports
        target_compile_definitions(${PROJECT_NAME} PRIVATE NGINE_SHARED=1)

        # Copy dll
        add_custom_command(TARGET NgineTest POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Ngine>
                $<TARGET_FILE_DIR:NgineTest>)
    endif()

    # Copy content
    add_custom_command(TARGET NgineTest PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_LIST_DIR}/content
            $<TARGET_FILE_DIR:NgineTest>/content)

    # Copy ES2 dlls
    if (${OPENGL_VERSION} MATCHES "ES2")
        add_custom_command(TARGET NgineTest POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libEGL.dll
                $<TARGET_FILE_DIR:NgineTest>)

        add_custom_command(TARGET NgineTest POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/libGLESv2.dll
                $<TARGET_FILE_DIR:NgineTest>)

        add_custom_command(TARGET NgineTest POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${PROJECT_SOURCE_DIR}/third-party/ANGLE/lib/x86/d3dcompiler_47.dll
                $<TARGET_FILE_DIR:NgineTest>)
    endif()
elseif(${PLATFORM} MATCHES "UWP")
    if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
        message(FATAL_ERROR "Not building as Windows Store")
    endif()

    # Configure the package manifest
    configure_file(
            ${PROJECT_SOURCE_DIR}/cmake/UWP/Package.appxmanifest.in
            ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest
            @ONLY)

    # Configure content files
    set(CONTENT_FILES ${CONTENT_FILES}
            ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest
            )

    # Configure asset files TODO: content folder and loading
    set(ASSET_FILES ${ASSET_FILES}
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/SplashScreen.png
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/Square44x44Logo.png
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/Square150x150Logo.png
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/Wide310x150Logo.png
            )

    # Source Groups
    source_group("Source Files" FILES ${SOURCE_FILES})
    source_group("Header Files" FILES ${HEADER_FILES})
    source_group("Resource Files" FILES ${RESOURCE_FILES})

    # Add exe
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES} ${RESOURCE_FILES})
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_WINRT_COMPONENT TRUE)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

    # Set graphics version
    target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHICS_OPENGLES2=1)

    # Link Ngine
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
    target_link_libraries(${PROJECT_NAME} Ngine)

    # Link WindowsApp
    target_link_libraries(${PROJECT_NAME} WindowsApp)
endif()