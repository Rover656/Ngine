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
    add_custom_command(TARGET NgineTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Ngine>
            $<TARGET_FILE_DIR:NgineTest>)

    if (${OPENGL_VERSION} MATCHES "ES2")
        # Copy dependant dlls
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
endif()

# Copy content
add_custom_command(TARGET NgineTest PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/content
        $<TARGET_FILE_DIR:NgineTest>/content)