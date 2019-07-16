# Config

# raylib
set(BUILD_EXAMPLES OFF CACHE INTERNAL "Don't build raylib examples" FORCE)
set(BUILD_GAMES OFF CACHE INTERNAL "Don't build raylib games" FORCE)

if (${BUILD_SHARED})
    # Force raylib to build shared
    set(BUILD_SHARED_LIBS ON CACHE INTERNAL "Build as a shared library" FORCE)
    set(SHARED ON CACHE INTERNAL "Build raylib as a shared library" FORCE)
endif()

# Raylib options
set(SUPPORT_FILEFORMAT_BMP ON CACHE INTERNAL "Support this file format" FORCE)
set(SUPPORT_FILEFORMAT_TGA ON CACHE INTERNAL "Support this file format" FORCE)
set(SUPPORT_FILEFORMAT_JPG ON CACHE INTERNAL "Support this file format" FORCE)
set(SUPPORT_FILEFORMAT_GIF ON CACHE INTERNAL "Support this file format" FORCE)
set(SUPPORT_FILEFORMAT_PSD ON CACHE INTERNAL "Support this file format" FORCE)
set(SUPPORT_FILEFORMAT_PKM ON CACHE INTERNAL "Support this file format" FORCE)
set(SUPPORT_FILEFORMAT_PVR ON CACHE INTERNAL "Support this file format" FORCE)
set(SUPPORT_FILEFORMAT_FLAC ON CACHE INTERNAL "Support this format" FORCE)

# Add libraries
add_subdirectory("raylib")
add_subdirectory("Box2D-cmake")

# Set output directories
if (${BUILD_SHARED})
    set_target_properties(raylib
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine"
            )
else()
    set_target_properties(raylib_static
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine"
            )
endif()