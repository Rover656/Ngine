Helpful Macros
==============

[TOC]

Running an Ngine game works differently across different platforms, so Ngine provides the following macros to ensure that development is as easy as possible.

# NGINE_GAME_ENTRY
NGINE_GAME_ENTRY defines the entrypoint for a program (i.e. int main() on linux).
This makes it easier to develop for multiple platforms as less code needs to be written specifically for a platform.

# NGINE_RUN_GAME
NGINE_RUN_GAME runs a Ngine game for you. This is because on UWP, game running is more sophisticated. Using this macro makes your game compatible with UWP builds.