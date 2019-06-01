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
        if(MSVC AND NOT ${CMAKE_MAKE_PROGRAM} MATCHES "ninja.exe")
            add_custom_command(TARGET NgineTest POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_BINARY_DIR}/Ngine/$<CONFIG>/Ngine.dll"
                    "${CMAKE_BINARY_DIR}/NgineTest/$<CONFIG>/Ngine.dll")
            add_custom_command(TARGET NgineTest POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_BINARY_DIR}/Ngine/$<CONFIG>/raylib.dll"
                    "${CMAKE_BINARY_DIR}/NgineTest/$<CONFIG>/raylib.dll")
        else()
            add_custom_command(TARGET NgineTest POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_BINARY_DIR}/Ngine/Ngine.dll"
                    "${CMAKE_BINARY_DIR}/NgineTest/Ngine.dll")
            add_custom_command(TARGET NgineTest POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${CMAKE_BINARY_DIR}/Ngine/raylib.dll"
                    "${CMAKE_BINARY_DIR}/NgineTest/raylib.dll")
        endif()
    else()
        add_custom_command(TARGET NgineTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_BINARY_DIR}/Ngine/Ngine.so"
                "${CMAKE_BINARY_DIR}/NgineTest/Ngine.so")

        add_custom_command(TARGET NgineTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_BINARY_DIR}/Ngine/raylib.so"
                "${CMAKE_BINARY_DIR}/NgineTest/raylib.so")
    endif()
endif()

# Copy content
add_custom_command(TARGET NgineTest PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/test/content"
        "${CMAKE_BINARY_DIR}/NgineTest/content")