# Add executable
add_executable(NgineTest WIN32 entrypoint.cpp)

# Include directories
target_include_directories(Ngine PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Link libraries
target_link_libraries(NgineTest Ngine)

# Set output directory
set_target_properties(NgineTest
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/NgineTest"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/NgineTest"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/NgineTest"
)

# Copy dependant dlls
add_custom_command(TARGET NgineTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/Ngine/Ngine.dll"
        "${CMAKE_BINARY_DIR}/NgineTest/Ngine.dll")

add_custom_command(TARGET NgineTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/Ngine/raylib.dll"
        "${CMAKE_BINARY_DIR}/NgineTest/raylib.dll")