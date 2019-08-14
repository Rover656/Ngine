# Fetch source files
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE CONTENT_FILES "content/*")

# Product information (Some for UWP)
set(PROJECT_NAME NgineTest)
set(SHORT_NAME ${PROJECT_NAME})
set(PACKAGE_GUID "62ab7c15-145e-4798-a843-a771f4ef8e9a")
set(PUBLISHER "NerdThings")
set(PUBLISHER_DISPLAY_NAME "NerdThings")
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

if (${PLATFORM} MATCHES "Desktop")
    if (${CMAKE_SYSTEM_NAME} STREQUAL "WindowsStore")
        message(FATAL_ERROR "Cannot build for Desktop under UWP. Use -DPLATFORM=UWP instead.")
    endif()

    # Add EXE
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})

    # Link Ngine
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
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

    # Defines
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_DESKTOP=1)

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

    # Configure asset files TODO: content folder and loading
    set(ASSET_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/SplashScreen.png
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/Square44x44Logo.png
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/Square150x150Logo.png
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/Wide310x150Logo.png
            )

    set(RESOURCE_FILES ${ASSET_FILES} ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest)

    # Mark for deploy
    set_property(SOURCE ${CONTENT_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)

    foreach(CONTENT_FILE ${CONTENT_FILES})
        # Get filename
        get_filename_component(CONTENT_FILENAME ${CONTENT_FILE} NAME)

        # Remove initial path
        string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" CONTENT_FILE_DEPLOYLOC ${CONTENT_FILE})

        # Remove filename
        string(REPLACE "/${CONTENT_FILENAME}" "" CONTENT_FILE_DEPLOYLOC ${CONTENT_FILE_DEPLOYLOC})

        message(${CONTENT_FILE})
        message(${CONTENT_FILE_DEPLOYLOC})
        set_property(SOURCE ${CONTENT_FILE} PROPERTY VS_DEPLOYMENT_LOCATION ${CONTENT_FILE_DEPLOYLOC})
    endforeach()

    set_property(SOURCE ${RESOURCE_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)
    set_property(SOURCE ${RESOURCE_FILES} PROPERTY VS_DEPLOYMENT_LOCATION "Assets")

    # Source Groups
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/content PREFIX "\\Content" FILES ${CONTENT_FILES})
    source_group("Assets" FILES ${RESOURCE_FILES})

    # Add exe
    add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES} ${RESOURCE_FILES} ${CONTENT_FILES})
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_WINRT_COMPONENT TRUE)
    #set_property(DIRECTORY ${CMAKE_BINARY_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

    # Set graphics version
    target_compile_definitions(${PROJECT_NAME} PRIVATE GRAPHICS_OPENGLES2=1)

    # Link Ngine
    target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src)
    target_link_libraries(${PROJECT_NAME} Ngine)

    # Link WindowsApp
    target_link_libraries(${PROJECT_NAME} WindowsApp)

    # Configure the script ready for the project mod
    set(PROJECT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    configure_file(${PROJECT_SOURCE_DIR}/cmake/UWP/UWPConfigureANGLEProject.ps1.in
            ${CMAKE_BINARY_DIR}/Configure/UWPConfigureANGLE${PROJECT_NAME}.ps1)
    configure_file(${PROJECT_SOURCE_DIR}/cmake/UWP/UWPConfigureProject.ps1.in
            ${CMAKE_BINARY_DIR}/Configure/UWPConfigure${PROJECT_NAME}.ps1)

    # Defines
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_UWP=1)
endif()