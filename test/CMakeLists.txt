# Add executable
add_executable(NgineTest WIN32 entrypoint.cpp)

# Include directories
target_include_directories(Ngine PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Link libraries
target_link_libraries(NgineTest Ngine)

# Use Ngine shared if building as shared
if (${BUILD_SHARED})
    target_compile_definitions(NgineTest PRIVATE NGINE_SHARED=1)
endif()

# Set output directory
set_target_properties(NgineTest
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/NgineTest"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/NgineTest"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/NgineTest"
)

# Copy dependant dlls
if (${BUILD_SHARED})
    if (WIN32)
        add_custom_command(TARGET NgineTest POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Ngine>
                $<TARGET_FILE_DIR:NgineTest>)

        add_custom_command(TARGET NgineTest POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:raylib>
                $<TARGET_FILE_DIR:NgineTest>)
    else()
        add_custom_command(TARGET NgineTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Ngine>
                $<TARGET_FILE_DIR:NgineTest>)

        add_custom_command(TARGET NgineTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:raylib_static>
                $<TARGET_FILE_DIR:NgineTest>)
    endif()
endif()

# Copy content
add_custom_command(TARGET NgineTest PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/test/content"
        "${CMAKE_BINARY_DIR}/NgineTest/content")