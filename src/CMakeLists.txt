# Fetch source files
file(GLOB_RECURSE _NGINE_SRC "${Ngine_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE _NGINE_PRIV_HEADERS "${Ngine_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE _NGINE_HEADERS "${Ngine_SOURCE_DIR}/include/*.hpp")

# Setup extras
set(_NGINE_PUBLIC_INCLUDE_DIRS "")
set(_NGINE_PRIVATE_INCLUDE_DIRS "")
set(_NGINE_LIBRARIES "")

# Configure config
configure_file(${PROJECT_SOURCE_DIR}/include/ngine/config.hpp.in ${Ngine_BINARY_DIR}/include/ngine/config.hpp)

# GLAD
if (NOT PLATFORM_UWP)
    if (NGINE_ENABLE_OPENGL OR NGINE_ENABLE_OPENGLES)
        set(_NGINE_SRC ${_NGINE_SRC} ${Ngine_SOURCE_DIR}/vendor/glad/src/glad.c)
        set(_NGINE_PRIVATE_INCLUDE_DIRS ${_NGINE_PRIVATE_INCLUDE_DIRS} ${Ngine_SOURCE_DIR}/vendor/glad/include)
    endif()
endif()

# GLFW
if (PLATFORM_DESKTOP)
    set(_NGINE_PRIVATE_INCLUDE_DIRS ${_NGINE_PRIVATE_INCLUDE_DIRS} ${Ngine_SOURCE_DIR}/vendor/glfw/include)
    set(_NGINE_LIBRARIES ${_NGINE_LIBRARIES} glfw)
endif()

# DirectX
if (NGINE_ENABLE_DIRECTX)
    set(_NGINE_LIBRARIES ${_NGINE_LIBRARIES} d3d11.lib dxgi.lib d3dcompiler.lib)
endif()

# ANGLE
if (NGINE_ENABLE_OPENGLES)
    if (PLATFORM_DESKTOP)
        set(_NGINE_LIBRARIES ${_NGINE_LIBRARIES} ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/libEGL.lib
                                                 ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/libGLESv2.lib)
        set(_NGINE_PRIVATE_INCLUDE_DIRS ${_NGINE_PRIVATE_INCLUDE_DIRS} ${Ngine_SOURCE_DIR}/vendor/ANGLE/include)
    elseif (PLATFORM_UWP)
        set(_NGINE_LIBRARIES ${_NGINE_LIBRARIES} ${CMAKE_CURRENT_BINARY_DIR}/packages/ANGLE.WindowsStore.2.1.13/build/native/ANGLE.WindowsStore.targets)
        set(_NGINE_PRIVATE_INCLUDE_DIRS ${_NGINE_PRIVATE_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/packages/ANGLE.WindowsStore.2.1.13/include)
    endif()
endif()

# UWP
if (PLATFORM_UWP)
    # No warnings
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Setup projects
if (BUILD_NGINE_STATIC)
    # Add library
    add_library(Ngine_static ${_NGINE_SRC} ${_NGINE_PRIV_HEADERS} ${_NGINE_HEADERS})

    # Setup language features
    target_compile_features(Ngine_static PUBLIC cxx_std_17)

    # Include directories and linkages
    target_link_libraries(Ngine_static
            PRIVATE
            box2d
            ${_NGINE_LIBRARIES})
    target_include_directories(Ngine_static
            PUBLIC
            "${Ngine_SOURCE_DIR}/include"
            "${Ngine_BINARY_DIR}/include" # Add the configured config.hpp
            "${Ngine_SOURCE_DIR}/vendor/miniaudio"
            ${_NGINE_PUBLIC_INCLUDE_DIRS}
            PRIVATE
            "${Ngine_SOURCE_DIR}/src"
            "${Ngine_SOURCE_DIR}/vendor/stb"
            ${_NGINE_PRIVATE_INCLUDE_DIRS})

    if (PLATFORM_UWP)
        # Mark as WinRT Component
        set_property(TARGET Ngine_static PROPERTY VS_WINRT_COMPONENT TRUE)
    endif ()

    if (WIN32 AND PLATFORM_DESKTOP)
        target_compile_definitions(Ngine_static PRIVATE _WIN32_WINNT=0x0A000006)
    endif ()

    # ANGLE
    if (NGINE_ENABLE_OPENGLES)
        if (PLATFORM_DESKTOP)
            # Copy DLLs to output
            add_custom_command(TARGET Ngine_static
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/libEGL.dll
                    $<TARGET_FILE_DIR:Ngine_static>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/libGLESv2.dll
                    $<TARGET_FILE_DIR:Ngine_static>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/d3dcompiler_47.dll
                    $<TARGET_FILE_DIR:Ngine_static>
                    COMMENT "Copying ANGLE binaries to output.")
        elseif(PLATFORM_UWP)
            # Install ANGLE with Nuget
            add_custom_target(NgineInstallANGLEUWP
                    COMMAND ${Ngine_SOURCE_DIR}/vendor/windows-redist/nuget.exe install ANGLE.WindowsStore -Version 2.1.13 -OutputDirectory ${CMAKE_CURRENT_BINARY_DIR}/packages
                    COMMENT "Installing ANGLE.WindowStore V2.1.13")
            add_dependencies(Ngine_static NgineInstallANGLEUWP)
        endif()
    endif()
endif()

if (BUILD_NGINE_SHARED)
    # Add library
    add_library(Ngine SHARED ${_NGINE_SRC} ${_NGINE_PRIV_HEADERS} ${_NGINE_HEADERS})

    # Mark for exporting
    target_compile_definitions(Ngine PRIVATE NGINE_EXPORTS=1)

    # Setup language features
    target_compile_features(Ngine PUBLIC cxx_std_17)

    # Include directories and linkages
    target_link_libraries(Ngine
            PRIVATE
            box2d
            ${_NGINE_LIBRARIES})
    target_include_directories(Ngine
            PUBLIC
            "${Ngine_SOURCE_DIR}/include"
            "${Ngine_BINARY_DIR}/include" # Add the configured config.hpp
            "${Ngine_SOURCE_DIR}/vendor/miniaudio"
            ${_NGINE_PUBLIC_INCLUDE_DIRS}
            PRIVATE
            "${Ngine_SOURCE_DIR}/src"
            "${Ngine_SOURCE_DIR}/vendor/stb"
            ${_NGINE_PRIVATE_INCLUDE_DIRS})

    if (PLATFORM_UWP)
        # Mark as WinRT Component
        set_property(TARGET Ngine PROPERTY VS_WINRT_COMPONENT TRUE)
    endif ()

    if (WIN32 AND PLATFORM_DESKTOP)
        target_compile_definitions(Ngine PRIVATE _WIN32_WINNT=0x0A000006)
    endif ()

    # ANGLE
    if (NGINE_ENABLE_OPENGLES)
        if (PLATFORM_DESKTOP)
            # Copy DLLs to output
            add_custom_command(TARGET Ngine
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/libEGL.dll
                    $<TARGET_FILE_DIR:Ngine>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/libGLESv2.dll
                    $<TARGET_FILE_DIR:Ngine>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${Ngine_SOURCE_DIR}/vendor/ANGLE/WindowsBinaries/x86/d3dcompiler_47.dll
                    $<TARGET_FILE_DIR:Ngine>
                    COMMENT "Copying ANGLE binaries to output.")
        elseif(PLATFORM_UWP)
            # Install ANGLE with Nuget
            add_custom_target(NgineInstallANGLEUWP
                    COMMAND ${Ngine_SOURCE_DIR}/vendor/windows-redist/nuget.exe install ANGLE.WindowsStore -Version 2.1.13 -OutputDirectory ${CMAKE_CURRENT_BINARY_DIR}/packages
                    COMMENT "Installing ANGLE.WindowStore V2.1.13")
            add_dependencies(Ngine NgineInstallANGLEUWP)
        endif()
    endif()
endif()

# TODO: Doxygen