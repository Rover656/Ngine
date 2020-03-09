/**********************************************************************************************
 *
 *   Ngine - A 2D game engine.
 *
 *   Copyright 2020 NerdThings (Reece Mackie)
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 **********************************************************************************************/

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