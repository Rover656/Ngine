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

# Include Ngine cmake functions
include(Ngine)

# Check config is ok
ngine_check_config(${PLATFORM})

# Get UWP extras TODO: Add a function for this??
if (${PLATFORM} MATCHES "UWP")
    # Configure asset files
    set(ASSET_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/SplashScreen.png
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/Square44x44Logo.png
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/Square150x150Logo.png
            ${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/Wide310x150Logo.png
            )
    set(RESOURCE_FILES ${ASSET_FILES} ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest)
endif()

# Configure UWP
ngine_configure_uwp_executable(${PROJECT_SOURCE_DIR}/cmake/UWP/Package.appxmanifest.in ${PROJECT_NAME} ${PACKAGE_GUID} ${PUBLISHER} ${PUBLISHER_DISPLAY_NAME} ${VERSION_MAJOR} ${VERSION_MINOR} ${VERSION_PATCH})

# Visual Studio Source Groups
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES} ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/content PREFIX "\\Content" FILES ${CONTENT_FILES})
source_group("Assets" FILES ${RESOURCE_FILES})

# Add EXE
ngine_add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${CONTENT_FILES} ${RESOURCE_FILES})

# Link Ngine
ngine_link_ngine(${PROJECT_NAME})

# Add content
ngine_add_content(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR} content ${RESOURCE_FILES})