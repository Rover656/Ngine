/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*  
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: At the end of this file.
*
**********************************************************************************************/

#ifndef NGINE_HPP
#define NGINE_HPP

#include "Config.hpp"

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
 * To build on linux, it is recommended to use GCC 7 or later.
 * The following packages need installed (example here is for linux):
 * @code
 * sudo apt-get update
 * sudo apt-get install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
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
// Namespace Documentation
//----------------------------------------------------------------------------------

/**
 * @namespace NerdThings::Ngine
 * The main Ngine namespace. Contains everything as part of the engine itself.
 *
 * @namespace NerdThings::Ngine::Audio
 * Ngine Audio module.
 *
 * @namespace NerdThings::Ngine::Components
 * This namespace consists of useful components.
 *
 * @namespace NerdThings::Ngine::Filesystem
 * Ngine Filesystem module.
 *
 * @namespace NerdThings::Ngine::Graphics
 * Ngine Graphics module.
 *
 * @namespace NerdThings::Ngine::Input
 * Ngine Input module.
 *
 * @namespace NerdThings::Ngine::Physics
 * Ngine Physics module.
 *
 * @namespace NerdThings::Ngine::Physics::Shapes
 * Physics shapes.
 *
 * @namespace NerdThings::Ngine::UI
 * Ngine UI module. This entire module is pending a rewrite.
 *
 * @namespace NerdThings::Ngine::UWP
 * This namespace contains UWP specific code.
 */

// ---------------------------------------------------------------------------------
// Ngine includes
// ---------------------------------------------------------------------------------

#include "Audio/AudioBuffer.hpp"
#include "Audio/AudioDevice.hpp"
#include "Audio/AudioStream.hpp"
#include "Audio/Music.hpp"
#include "Audio/Sound.hpp"
#include "Audio/Wave.hpp"
#include "Components/CameraComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/TilesetComponent.hpp"
#include "Components/UIWidgetComponent.hpp"
#include "Filesystem/Filesystem.hpp"
#include "Filesystem/ResourceManager.hpp"
#include "Graphics/Rendering/QuadRenderable.hpp"
#include "Graphics/Rendering/Renderable.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Canvas.hpp"
#include "Graphics/Color.hpp"
#include "Graphics/Font.hpp"
#include "Graphics/GraphicsDevice.hpp"
#include "Graphics/GraphicsManager.hpp"
#include "Graphics/Image.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/RenderTarget.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Graphics/Sprite.hpp"
#include "Graphics/Texture2D.hpp"
#include "Graphics/Tileset.hpp"
#include "Graphics/TilesetRenderer.hpp"
#include "Input/Gamepad.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"
// TODO: Physics once we have finished the API in this branch
// TODO: UI once the API is finished
#include "Component.hpp"
#include "Entity.hpp"
#include "EntityContainer.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "Logger.hpp"
#include "Math.hpp"
#include "Rectangle.hpp"
#include "Resource.hpp"
#include "Scene.hpp"
#include "Window.hpp"

// ---------------------------------------------------------------------------------
// Platform specific features
// ---------------------------------------------------------------------------------

#if defined(PLATFORM_UWP)
#include "UWP/GameApp.hpp"
#endif

// ---------------------------------------------------------------------------------
// Handy macros
// ---------------------------------------------------------------------------------

/*
 * These macros are here to make cross platform coding easier.
 */

// Game entrypoint
#if defined(PLATFORM_DESKTOP)
#if defined(_WIN32)
#define NGINE_GAME_ENTRY int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
#else
#define NGINE_GAME_ENTRY int main()
#endif
#elif defined(PLATFORM_UWP)
#define NGINE_GAME_ENTRY [Platform::MTAThread] int main(Platform::Array<Platform::String^>^)
#endif

// Game runner
#if defined(PLATFORM_UWP)
#include "UWP/GameApp.hpp"
#define NGINE_RUN_GAME(game) UWP::UWPGameBootstrapper::ExecuteGame(&game); CoreApplication::Run(ref new UWP::GameApplicationSource(ref new UWP::GameApp()));
#else
#define NGINE_RUN_GAME(game) game.Run();
#endif

#endif // NGINE_HPP

/*
                   GNU LESSER GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.


  This version of the GNU Lesser General Public License incorporates
the terms and conditions of version 3 of the GNU General Public
License, supplemented by the additional permissions listed below.

  0. Additional Definitions.

  As used herein, "this License" refers to version 3 of the GNU Lesser
General Public License, and the "GNU GPL" refers to version 3 of the GNU
General Public License.

  "The Library" refers to a covered work governed by this License,
other than an Application or a Combined Work as defined below.

  An "Application" is any work that makes use of an interface provided
by the Library, but which is not otherwise based on the Library.
Defining a subclass of a class defined by the Library is deemed a mode
of using an interface provided by the Library.

  A "Combined Work" is a work produced by combining or linking an
Application with the Library.  The particular version of the Library
with which the Combined Work was made is also called the "Linked
Version".

  The "Minimal Corresponding Source" for a Combined Work means the
Corresponding Source for the Combined Work, excluding any source code
for portions of the Combined Work that, considered in isolation, are
based on the Application, and not on the Linked Version.

  The "Corresponding Application Code" for a Combined Work means the
object code and/or source code for the Application, including any data
and utility programs needed for reproducing the Combined Work from the
Application, but excluding the System Libraries of the Combined Work.

  1. Exception to Section 3 of the GNU GPL.

  You may convey a covered work under sections 3 and 4 of this License
without being bound by section 3 of the GNU GPL.

  2. Conveying Modified Versions.

  If you modify a copy of the Library, and, in your modifications, a
facility refers to a function or data to be supplied by an Application
that uses the facility (other than as an argument passed when the
facility is invoked), then you may convey a copy of the modified
version:

   a) under this License, provided that you make a good faith effort to
   ensure that, in the event an Application does not supply the
   function or data, the facility still operates, and performs
   whatever part of its purpose remains meaningful, or

   b) under the GNU GPL, with none of the additional permissions of
   this License applicable to that copy.

  3. Object Code Incorporating Material from Library Header Files.

  The object code form of an Application may incorporate material from
a header file that is part of the Library.  You may convey such object
code under terms of your choice, provided that, if the incorporated
material is not limited to numerical parameters, data structure
layouts and accessors, or small macros, inline functions and templates
(ten or fewer lines in length), you do both of the following:

   a) Give prominent notice with each copy of the object code that the
   Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the object code with a copy of the GNU GPL and this license
   document.

  4. Combined Works.

  You may convey a Combined Work under terms of your choice that,
taken together, effectively do not restrict modification of the
portions of the Library contained in the Combined Work and reverse
engineering for debugging such modifications, if you also do each of
the following:

   a) Give prominent notice with each copy of the Combined Work that
   the Library is used in it and that the Library and its use are
   covered by this License.

   b) Accompany the Combined Work with a copy of the GNU GPL and this license
   document.

   c) For a Combined Work that displays copyright notices during
   execution, include the copyright notice for the Library among
   these notices, as well as a reference directing the user to the
   copies of the GNU GPL and this license document.

   d) Do one of the following:

       0) Convey the Minimal Corresponding Source under the terms of this
       License, and the Corresponding Application Code in a form
       suitable for, and under terms that permit, the user to
       recombine or relink the Application with a modified version of
       the Linked Version to produce a modified Combined Work, in the
       manner specified by section 6 of the GNU GPL for conveying
       Corresponding Source.

       1) Use a suitable shared library mechanism for linking with the
       Library.  A suitable mechanism is one that (a) uses at run time
       a copy of the Library already present on the user's computer
       system, and (b) will operate properly with a modified version
       of the Library that is interface-compatible with the Linked
       Version.

   e) Provide Installation Information, but only if you would otherwise
   be required to provide such information under section 6 of the
   GNU GPL, and only to the extent that such information is
   necessary to install and execute a modified version of the
   Combined Work produced by recombining or relinking the
   Application with a modified version of the Linked Version. (If
   you use option 4d0, the Installation Information must accompany
   the Minimal Corresponding Source and Corresponding Application
   Code. If you use option 4d1, you must provide the Installation
   Information in the manner specified by section 6 of the GNU GPL
   for conveying Corresponding Source.)

  5. Combined Libraries.

  You may place library facilities that are a work based on the
Library side by side in a single library together with other library
facilities that are not Applications and are not covered by this
License, and convey such a combined library under terms of your
choice, if you do both of the following:

   a) Accompany the combined library with a copy of the same work based
   on the Library, uncombined with any other library facilities,
   conveyed under the terms of this License.

   b) Give prominent notice with the combined library that part of it
   is a work based on the Library, and explaining where to find the
   accompanying uncombined form of the same work.

  6. Revised Versions of the GNU Lesser General Public License.

  The Free Software Foundation may publish revised and/or new versions
of the GNU Lesser General Public License from time to time. Such new
versions will be similar in spirit to the present version, but may
differ in detail to address new problems or concerns.

  Each version is given a distinguishing version number. If the
Library as you received it specifies that a certain numbered version
of the GNU Lesser General Public License "or any later version"
applies to it, you have the option of following the terms and
conditions either of that published version or of any later version
published by the Free Software Foundation. If the Library as you
received it does not specify a version number of the GNU Lesser
General Public License, you may choose any version of the GNU Lesser
General Public License ever published by the Free Software Foundation.

  If the Library as you received it specifies that a proxy can decide
whether future versions of the GNU Lesser General Public License shall
apply, that proxy's public statement of acceptance of any version is
permanent authorization for you to choose that version for the
Library.
 */