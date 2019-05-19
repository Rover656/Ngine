# Config

# raylib
set(BUILD_EXAMPLES OFF CACHE INTERNAL "Don't build raylib examples" FORCE)
set(BUILD_GAMES OFF CACHE INTERNAL "Don't build raylib games" FORCE)

if (${BUILD_SHARED})
	# Force raylib to build shared
	set(BUILD_SHARED_LIBS ON CACHE INTERNAL "Build as a shared library" FORCE)
	set(SHARED ON CACHE INTERNAL "Build raylib as a shared library" FORCE)
endif()

# Add libraries
add_subdirectory("raylib")

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