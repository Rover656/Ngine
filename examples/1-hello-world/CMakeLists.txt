# Include Ngine
include (Ngine)

project(ngine_ex1_helloworld)

# Add game
add_game(
        NAME ${PROJECT_NAME}
        TARGET_NAME ${PROJECT_NAME}
        SOURCE_FILES entry.cpp
        PUBLISHER_NAME "NerdThings"
        CONTENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../content"
)