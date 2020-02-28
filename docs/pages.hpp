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
 * @namespace ngine::graphics::API
 * Ngine platform agnostic APIs.
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
