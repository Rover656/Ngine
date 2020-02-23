# Fetch source files
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.hpp")

# Include Ngine cmake functions
include(Ngine)

project(NgineTest
        VERSION 1.0.0
        DESCRIPTION "A test program to see if the game engine performs given tasks. To be replaced with examples.")

# Get UWP resources
if (PLATFORM MATCHES "UWP")
    # Configure asset files
    file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Assets.UWP/*")
endif()

add_game(
        NAME ${PROJECT_NAME}
        TARGET_NAME "NgineTest"

        VERSION_MAJOR ${NgineTest_VERSION_MAJOR}
        VERSION_MINOR ${NgineTest_VERSION_MINOR}
        VERSION_PATCH ${NgineTest_VERSION_PATCH}

        PUBLISHER_NAME "NerdThings"
        SOURCE_FILES ${SOURCE_FILES}
        RESOURCE_FILES ${RESOURCE_FILES}
        UWP_PACKAGE_GUID "62ab7c15-145e-4798-a843-a771f4ef8e9a"
        )

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "\\Source Files" FILES ${SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "\\Header Files" FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/content PREFIX "\\Content" FILES ${CONTENT_FILES})