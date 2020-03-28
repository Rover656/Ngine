add_executable(NgineTest WIN32 test.cpp)
target_link_libraries(NgineTest Ngine_static)

if (PLATFORM_UWP)
    target_link_libraries(NgineTest WindowsApp)
    set_property(TARGET NgineTest PROPERTY VS_WINRT_COMPONENT TRUE)
endif()