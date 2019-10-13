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
ngine_check_config()

# Get UWP resources
if (${PLATFORM} MATCHES "UWP")
    # Configure asset files
    file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/*")
endif()

ngine_add_game(
        NAME "TestGame"
        TARGET_NAME "NgineTest"
        SOURCE_FILES ${SOURCE_FILES}
        RESOURCE_FILES ${RESOURCE_FILES}
        UWP_PACKAGE_GUID "62ab7c15-145e-4798-a843-a771f4ef8e9a"
        UWP_PUBLISHER_DISPLAY_NAME "NerdThings"
        )