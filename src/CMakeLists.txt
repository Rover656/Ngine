# Fetch source files
file(GLOB_RECURSE Ngine_source
    "*.h"
    "*.cpp"
)

# Generate product info
include(generate_product_version)
generate_product_version(
        VersionFilesOutputVariable
        NAME "Ngine"
        VERSION_MAJOR 0
        VERSION_MINOR 1
        VERSION_PATCH 0
        VERSION_REVISION 0
        COMPANY_NAME "NerdThings"
)

# Add library
if (${BUILD_SHARED})
    message("Building Ngine as a shared library.")
    add_library(Ngine SHARED ${Ngine_source} ${VersionFilesOutputVariable})
else()
    message("Building Ngine as a static library.")
    add_library(Ngine STATIC ${Ngine_source} ${VersionFilesOutputVariable})
endif()

# Include directories
target_include_directories(Ngine PUBLIC "3rd-party/raylib/src")
target_include_directories(Ngine PUBLIC "3rd-party/Box2D-cmake/Box2D")

# Link libraries
if (${BUILD_SHARED})
    message("Linking raylib shared")
    target_link_libraries(Ngine raylib)
else()
    message("Linking raylib static")
    target_link_libraries(Ngine raylib_static)
endif()
target_link_libraries(Ngine tobanteGaming::Box2D)

if (UNIX OR MINGW)
    target_link_libraries(Ngine stdc++fs)
endif()

if (MINGW)
    target_link_libraries(Ngine -static)
endif()

# Compile definitions
target_compile_definitions(Ngine PRIVATE NGINE_EXPORTS=1)

if (${BUILD_SHARED})
    message("Using shared libtype")
    target_compile_definitions(Ngine PRIVATE USE_LIBTYPE_SHARED=1)
endif()