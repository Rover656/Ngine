/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Ngine.hpp"

#include "Config.hpp"

// STB Image

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// STB Truetype

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

// Miniaudio
#define MA_NO_JACK
#define MINIAUDIO_IMPLEMENTATION
#include <miniaudio.h>

// OGG Format
#define STB_VORBIS_IMPLEMENTATION
#include <stb_vorbis.h>

// FLAC FORMAT
#define DR_FLAC_IMPLEMENTATION
#define DR_FLAC_NO_WIN32_IO
#include <dr_flac.h>

// MP3 FORMAT
#define DR_MP3_IMPLEMENTATION
#include <dr_mp3.h>

// WAV FORMAT
#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>

// ConsoleMessage
void ConsoleMessage(std::string message, std::string severity, std::string module) {
    {
        std::cout << "[" + module + "] " + severity + ": " + message << std::endl;
    }
}
