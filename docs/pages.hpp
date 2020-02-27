// ---------------------------------------------------------------------------------
// Doxygen Pages
// ---------------------------------------------------------------------------------

/**
 * @mainpage Ngine API Documentation
 *
 * @section intro_sec Introduction
 * Ngine is a 2D game engine written in C++.
 *
 * @section installation Installation
 * NerdThings does not currently provide binaries for Ngine, you must build it from source instead.
 * Build instructions are available on the @ref platform_support "Platform Support" page
 *
 * @page platform_support Platform Support
 * @tableofcontents
 *
 * Ngine supports a wide range of platforms. This page details build instructions for each platform.
 * All build instructions are compatible with the CMake config guides.
 *
 * @section platform_win Windows (Native)
 * Ngine primarily supports native Windows games and therefore Windows will have every feature (where applicable).
 *
 * @subsection platform_win_graphics Graphics APIs
 * Windows supports both OpenGL and DirectX (via ANGLE) graphics APIs.
 *
 * @subsection platform_win_build Building
 * To build, you must have at least Visual Studio 2017 installed with relevant workloads. Once installed, run the VS command line and run the following:
 * MinGW support is untested.
 * @code
 * mkdir build & cd build
 * cmake .. <Insert any CMake config here>
 * cmake --build . --config <Debug or Release>
 * @endcode
 *
 * @section platform_linux Linux
 * All Ngine features are available on Linux.
 *
 * @subsection platform_linux_graphics Graphics APIs
 * Linux only supports the OpenGL graphics API.
 *
 * @subsection platform_linux_build Building
 * To build on linux, follow the following instructions.
 * The following packages need installed (example here is for linux):
 * @code
 * sudo apt-get update
 * sudo apt-get install build-essential libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev
 * @endcode
 * Ngine can then be built with the following command:
 * @code
 * mkdir build & cd build
 * cmake -DCMAKE_BUILD_TYPE=<Release or Debug> <Other CMake config> ..
 * make
 * @endcode
 *
 * @section platform_mac Mac OS X
 * Ngine does not currently support Mac OS X and it is not planned. Feel free to work on this and publish a PR if you decide to try!
 *
 * @section platform_uwp Windows UWP (Desktop, Mobile, Xbox One)
 * Ngine supports UWP across Desktop and Xbox One. No Mobile UWP features will be added as Windows 10 Mobile is discontinued.
 *
 * @subsection platform_uwp_graphics Graphics APIs
 * Ngine on UWP uses DirectX (ANGLE).
 *
 * @subsection platform_uwp_missingfeatures Missing Features
 * - Ngine does not support touch input at the moment.
 *
 * @subsection platform_uwp_build Building
 * Building for Ngine requires Visual Studio 2017 (2019 may work, but is untested) with UWP workloads.
 *
 * Ngine includes a build script `BuildUWP.bat` which will generate a directory `build-uwp` with visual studio projects.
 * It is recommended to implement this script for your own projects for ease.
 *
 * @section platform_other Not listed?
 * If a platform is not listed, it means it has not been considered yet, feel free to ask about this on the GitHub page.
 * It is planned to add easier platform porting in the future (allowing community ports to be made easily).
 *
 * @section cmake_options CMake Options
 *
 * | Option             | Description                                      | Default |
 * | ------------------ | ------------------------------------------------ | ------- |
 * | BUILD_TEST         | Build the test program                           | ON      |
 * | BUILD_DOCS         | Build the documentation (release only)           | ON      |
 * | BUILD_EXAMPLES     | Build example code                               | OFF     |
 * | BUILD_NGINE_SHARED | Build Ngine as a shared library                  | OFF     |
 * | PLATFORM           | The platform to build for (Desktop or UWP)       | Desktop |
 * | ENABLE_OPENGL      | Enable use of OpenGL (On supported platforms)    | ON      |
 * | ENABLE_OPENGLES    | Enable use of OpenGL ES (On suported platforms)  | ON      |
 *
 * @page helpful_macros Helpful Macros
 * @tableofcontents
 *
 * Running an Ngine game works differently across different platforms, so Ngine provides the following macros to ensure that development is as easy as possible.
 *
 * @section macro_NGINE_GAME_ENTRY NGINE_GAME_ENTRY
 * NGINE_GAME_ENTRY defines the entrypoint for a program (i.e. int main() on linux).
 * This makes it easier to develop for multiple platforms as less code needs to be written specifically for a platform.
 *
 * @section macro_NGINE_RUN_GAME NGINE_RUN_GAME
 * NGINE_RUN_GAME runs a Ngine game for you. This is because on UWP, game running is more sophisticated. Using this macro makes your game compatible with UWP builds.
 */

//----------------------------------------------------------------------------------
// Page: Code conventions
//----------------------------------------------------------------------------------

/**
 * @page code_conventions Code Conventions
 * @section naming Naming Convention
 * Private struct and class fields must have the `m_` prefix and be in `lowerCamelCase`.
 * 
 * Private struct and class methods must have the `_` prefix and be in `lowerCamelCase`.
 * 
 * Public struct and class fields and methods must be named in `UpperCamelCase`.
 * 
 * All struct, enum and class names must be be in `UpperCamelCase`.
 * 
 * All local variables must be in `lowerCamelCase`.
 * 
 * All parameters must have the `_` suffix and be in `lowerCamelCase`.
 * 
 * All namespaces are based on paths and are prefixed with `NerdThings::Ngine`.
 * 
 * @section comments Commenting Styles
 * Single line comments must have a space following the `//`.
 * Multi-line comments must have an `*` followed by a space before each line of text. E.g
 * @code
 * /&zwj;*
 *  * This is a multi-line comment
 *  *&zwj;/
 * @endcode
 * If you are commenting anything outside a function body, it must be doxygen compatible. E.g
 * @code
 * /&zwj;**
 *  * This function does stuff
 *  *
 *  * @param paramA_ The first parameter.
 *  * @return The answer.
 *  *&zwj;/
 *  std::string getTheAnswer(int paramA_);
 * @endcode
 */
 
//----------------------------------------------------------------------------------
// Namespace Documentation
//----------------------------------------------------------------------------------

/**
 * @namespace ngine
 * The main Ngine namespace. Contains everything as part of the engine itself.
 *
 * @namespace ngine::audio
 * Ngine Audio module.
 *
 * @namespace ngine::components
 * This namespace consists of useful components.
 *
 * @namespace ngine::filesystem
 * Ngine Filesystem module.
 *
 * @namespace ngine::graphics
 * Ngine Graphics module.
 *
 * @namespace ngine::input
 * Ngine Input module.
 *
 * @namespace ngine::physics
 * Ngine Physics module.
 *
 * @namespace ngine::UWP
 * This namespace contains UWP specific code.
 */
