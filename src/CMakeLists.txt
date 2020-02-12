# Fetch source files
file(GLOB_RECURSE SOURCE_FILES "${Ngine_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${Ngine_SOURCE_DIR}/include/*.hpp")

# Generate versioning
if (${PLATFORM} MATCHES "Desktop" AND WIN32 AND ${BUILD_NGINE_SHARED})
    # Generate windows product info
    include(generate_product_version)
    generate_product_version(
            _NGINE_EXTRA_FILES
            NAME Ngine
            VERSION_MAJOR ${Ngine_VERSION_MAJOR}
            VERSION_MINOR ${Ngine_VERSION_MINOR}
            VERSION_PATCH ${Ngine_VERSION_PATCH}
            VERSION_REVISION 0
            COMPANY_NAME NerdThings)
endif ()

# GLAD
set(_NGINE_GLAD OFF)
if (NOT ${PLATFORM} MATCHES "UWP") # UWP does not like GLAD...
    if (${ENABLE_OPENGL} OR ${ENABLE_OPENGLES})
        set(_NGINE_GLAD ON)
    endif ()
endif ()

# GLAD source
if (${_NGINE_GLAD})
    set(_NGINE_EXTRA_FILES ${_NGINE_EXTRA_FILES} ${Ngine_SOURCE_DIR}/third-party/glad/src/glad.c)
endif()

# Add target
if (${BUILD_NGINE_SHARED})
    # Add library
    add_library(Ngine SHARED ${SOURCE_FILES} ${HEADER_FILES} ${_NGINE_EXTRA_FILES})

    # Mark for exporting
    target_compile_definitions(Ngine PRIVATE NGINE_EXPORTS=1)
else ()
    # Add library
    add_library(Ngine STATIC ${SOURCE_FILES} ${HEADER_FILES} ${_NGINE_EXTRA_FILES})
endif ()

# C++17
target_compile_features(Ngine PRIVATE cxx_std_17)

if (${ENABLE_OPENGL} AND ${PLATFORM} MATCHES "Desktop")
    target_compile_definitions(Ngine PUBLIC API_OPENGL_ENABLED=1)
endif ()
if (${ENABLE_OPENGLES})
    target_compile_definitions(Ngine PUBLIC API_OPENGLES_ENABLED=1)
endif ()

# Depend on docs if enabled and we are able to say we're building release
if (${BUILD_DOCS} AND (DEFINED ${CMAKE_BUILD_TYPE} AND ${CMAKE_BUILD_TYPE} MATCHES "Release"))
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        add_dependencies(Ngine Ngine_Docs)
    endif ()
endif ()

# UWP Specifics
if (${PLATFORM} MATCHES "UWP")
    # Mark as WinRT Component
    set_property(TARGET Ngine PROPERTY VS_WINRT_COMPONENT TRUE)

    # Link special libraries
    target_link_libraries(Ngine PUBLIC WindowsApp)

    # No warnings
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif ()

# Setup libraries and include dirs
target_link_libraries(Ngine
        PRIVATE
        ${_NGINE_LIBRARIES}
        Box2D-cmake)
target_include_directories(Ngine
        PUBLIC
            "${Ngine_SOURCE_DIR}/include"
        PRIVATE
            "${Ngine_SOURCE_DIR}/src"
            "${Ngine_SOURCE_DIR}/third-party/stb"
            "${Ngine_SOURCE_DIR}/third-party/Box2D-cmake/Box2D"
            "${Ngine_SOURCE_DIR}/third-party/miniaudio")

# GLFW
if (${PLATFORM} MATCHES "Desktop")
    target_link_libraries(Ngine PRIVATE glfw)
    target_include_directories(Ngine PRIVATE ${Ngine_SOURCE_DIR}/third-party/glfw/include)
endif ()

# GLAD
if (${_NGINE_GLAD})
    target_include_directories(Ngine PRIVATE ${Ngine_SOURCE_DIR}/third-party/glad/include)
endif()

# Graphics API handling
if (${PLATFORM} MATCHES "Desktop")
    if (${ENABLE_OPENGLES} AND WIN32)
        # Copy DLLs to output
        add_custom_command(TARGET Ngine
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${Ngine_SOURCE_DIR}/third-party/ANGLE/WindowsBinaries/x86/libEGL.dll
                $<TARGET_FILE_DIR:Ngine>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${Ngine_SOURCE_DIR}/third-party/ANGLE/WindowsBinaries/x86/libGLESv2.dll
                $<TARGET_FILE_DIR:Ngine>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${Ngine_SOURCE_DIR}/third-party/ANGLE/WindowsBinaries/x86/d3dcompiler_47.dll
                $<TARGET_FILE_DIR:Ngine>
                COMMENT "Copying ANGLE binaries to output.")

        # Link
        target_link_libraries(Ngine PRIVATE
                ${Ngine_SOURCE_DIR}/third-party/ANGLE/WindowsBinaries/x86/libEGL.lib
                ${Ngine_SOURCE_DIR}/third-party/ANGLE/WindowsBinaries/x86/libGLESv2.lib)

        # Include
        target_include_directories(Ngine PRIVATE ${Ngine_SOURCE_DIR}/third-party/ANGLE/include)
    endif ()
elseif (${PLATFORM} MATCHES "UWP")
    if (${ENABLE_OPENGLES})
        # Install ANGLE with Nuget
        add_custom_target(NgineInstallANGLEUWP
                COMMAND ${Ngine_SOURCE_DIR}/redist/nuget.exe install ANGLE.WindowsStore -Version 2.1.13 -OutputDirectory ${CMAKE_CURRENT_BINARY_DIR}/packages
                COMMENT "Installing ANGLE.WindowStore V2.1.13")
        add_dependencies(Ngine NgineInstallANGLEUWP)

        # Link
        target_link_libraries(Ngine PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/packages/ANGLE.WindowsStore.2.1.13/build/native/ANGLE.WindowsStore.targets)
    endif ()
endif ()

# Source Groups TODO: Account for different directory structures
source_group(TREE ${Ngine_SOURCE_DIR}/src PREFIX "\\Source Files" FILES ${SOURCE_FILES})
source_group(TREE ${Ngine_SOURCE_DIR}/include PREFIX "\\Header Files" FILES ${HEADER_FILES})