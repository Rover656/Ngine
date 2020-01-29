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

#if defined(GRAPHICS_OPENGLES2) || defined(GRAPHICS_OPENGL21) || defined(GRAPHICS_OPENGL33)

#ifndef NGINE_OPENGL
#define NGINE_OPENGL

#include "../Config.hpp"

// OpenGL 2.1 works similarly to 3.3
#if defined(GRAPHICS_OPENGL21)
#define GRAPHICS_OPENGL33
#endif

// Platform specifics
#if defined(PLATFORM_DESKTOP)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#elif defined(PLATFORM_UWP)
#define GL_KHR_debug 0
#define GL_GLEXT_PROTOTYPES 1
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#endif

#if defined(PLATFORM_DESKTOP)
#include "../../../third-party/glfw/include/GLFW/glfw3.h"
#elif defined(PLATFORM_UWP)
#include "../../../third-party/ANGLE/angle_windowsstore.h"
#endif

// Fix any missing GL definitions
#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#endif
#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif
#ifndef GL_ETC1_RGB8_OES
#define GL_ETC1_RGB8_OES 0x8D64
#endif
#ifndef GL_COMPRESSED_RGB8_ETC2
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#endif
#ifndef GL_COMPRESSED_RGBA8_ETC2_EAC
#define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#endif
#ifndef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG 0x8C00
#endif
#ifndef GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8C02
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_4x4_KHR
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0x93b0
#endif
#ifndef GL_COMPRESSED_RGBA_ASTC_8x8_KHR
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR 0x93b7
#endif
#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif
#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

// OpenGL 2.1 defines
#if defined(GRAPHICS_OPENGL21)
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A
#endif

// GLES2 defines
#if defined(GRAPHICS_OPENGLES2)
// Extra defines
#define glClearDepth glClearDepthf
#define GL_READ_FRAMEBUFFER GL_FRAMEBUFFER
#define GL_DRAW_FRAMEBUFFER GL_FRAMEBUFFER
#endif

#endif // NGINE_OPENGL

#endif