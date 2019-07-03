# Fetch source files
file(GLOB Ngine_source
    "*.h"
    "*.cpp"
    "Audio/*.h"
    "Audio/*.cpp"
    "Components/*.h"
    "Components/*.cpp"
    "Core/*.h"
    "Core/*.cpp"
    "Graphics/*.h"
    "Graphics/*.cpp"
    "Input/*.h"
    "Input/*.cpp"
    "Math/*.h"
    "Math/*.cpp"
    "Physics/*.h"
    "Physics/*.cpp"
    "Types/*.h"
    "Types/*.cpp"
    "UI/*.h"
    "UI/*.cpp"
    "UI/Controls/*.h"
    "UI/Controls/*.cpp"
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
target_include_directories(Ngine PUBLIC "3rd-party/Box2D-cmake/Box2D")

# Link libraries
target_link_libraries(Ngine raylib)
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
    target_compile_definitions(Ngine PRIVATE USE_LIBTYPE_SHARED=1)
endif()