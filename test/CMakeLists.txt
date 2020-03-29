add_executable(NgineTest WIN32 test.cpp)
target_link_libraries(NgineTest Ngine_static)

if (PLATFORM_UWP)
    target_link_libraries(NgineTest WindowsApp)
    set_property(TARGET NgineTest PROPERTY VS_WINRT_COMPONENT TRUE)
endif()

#target_compile_definitions(NgineTest PRIVATE NGINE_SHARED)
add_dependencies(NgineTest Ngine_static)

#add_custom_command(TARGET Ngine_static
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/libEGL.dll
#        $<TARGET_FILE_DIR:Ngine_static>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/libGLESv2.dll
#        $<TARGET_FILE_DIR:Ngine_static>
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/d3dcompiler_47.dll
#        $<TARGET_FILE_DIR:Ngine_static>
#        COMMENT "Copying ANGLE binaries to NgineTest.")
#add_dependencies(CopyANGLENgineTest Ngine_static)
#add_dependencies(NgineTest CopyANGLENgineTest)