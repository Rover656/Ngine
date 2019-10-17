# Fetch source files
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h")

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

        VERSION_MAJOR 1
        VERSION_MINOR 0
        VERSION_PATCH 0

        PUBLISHER_NAME "NerdThings"
        SOURCE_FILES ${SOURCE_FILES}
        RESOURCE_FILES ${RESOURCE_FILES}
        UWP_PACKAGE_GUID "62ab7c15-145e-4798-a843-a771f4ef8e9a"
        )

if(MSVC)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "\\Source Files" FILES ${SOURCE_FILES})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "\\Header Files" FILES ${HEADER_FILES})
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/content PREFIX "\\Content" FILES ${CONTENT_FILES})
endif()