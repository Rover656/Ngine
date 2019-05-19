# Fetch source files
file(GLOB Ngine_source
    "*.h"
    "*.cpp"
	"Core/*.h"
	"Core/*.cpp"
	"Graphics/*.h"
	"Graphics/*.cpp"
	"Input/*.h"
	"Input/*.cpp"
	"Types/*.h"
	"Types/*.cpp"
)

# Include options
include("CMakeOptions.txt")

# Add 3rd-party libraries
add_subdirectory("3rd-party")

# Add library
if (${BUILD_SHARED})
	message("Building Ngine as a shared library.")
	add_library(Ngine SHARED ${Ngine_source})
else()
	message("Building Ngine as a static library.")
	add_library(Ngine STATIC ${Ngine_source})
endif()

# Include directories
target_include_directories(Ngine PUBLIC "3rd-party/raylib/src")

# Link libraries
target_link_libraries(Ngine raylib)

# Compile definitions
target_compile_definitions(Ngine PRIVATE NGINE_EXPORTS=1)

if (${BUILD_SHARED})
	target_compile_definitions(Ngine PRIVATE USE_LIBTYPE_SHARED=1)
endif()

# Set output directory
set_target_properties(Ngine
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Ngine"
)