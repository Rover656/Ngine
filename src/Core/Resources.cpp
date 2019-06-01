/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Resources.h"

// Platform specific includes
#if defined(_WIN32)
#include <Windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <linux/limits.h>
#define _GNU_SOURCE
#define __USE_GNU
#include <dlfcn.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <dlfcn.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <pwd.h>
#endif

namespace NerdThings::Ngine::Core {
    // Public Methods

    std::string Resources::GetExecutableDirectory(bool &success_) {
        const auto exePath = GetExecutablePath(success_);

        if (success_) {
            return std::string(GetDirectoryPath(exePath.c_str()));
        }
        return "";
    }

    std::string Resources::GetExecutablePath(bool &success_) {
        // https://github.com/cginternals/cpplocate/blob/master/source/liblocate/source/liblocate.c#L39
        // I trust this works as there are no issues about it...
        success_ = false;
        #if defined (_WIN32)

        char exePath[MAX_PATH];
        unsigned int len = GetModuleFileNameA(GetModuleHandleA(0x0), exePath, MAX_PATH);
        if (len == 0)
            return "";

        success_ = true;
        return std::string(exePath);

        #elif defined(__linux__)

        char exePath[PATH_MAX];
        int len = readlink("/proc/self/exe", exePath, PATH_MAX);
        if (len <= 0 || len == PATH_MAX)
            return "";

        success_ = true;
        return std::string(exePath);

        #elif defined(__APPLE__)

        char exePath[PATH_MAX];

        unsigned int len = (unsigned int)PATH_MAX;

        if (_NSGetExecutablePath(exePath, &len) == 0)
        {
            char * realPath = realpath(exePath, 0x0);

            if (realPath == 0x0)
            {
                return "";
            }

            auto pathStr = std::string(realPath);

            free(realPath);

            success_ = true;
            return pathStr;
        }
        else
        {
            char * intermediatePath = (char *)malloc(sizeof(char) * len);

            if (_NSGetExecutablePath(intermediatePath, &len) != 0)
            {
                free(intermediatePath);
                return "";
            }

            char * realPath = realpath(intermediatePath, 0x0);

            free(intermediatePath);

            if (realPath == 0x0)
            {
                return "";
            }

            auto pathStr = std::string(realPath);

            free(realPath);

            success_ = true;
            return pathStr;
        }

        #else
        // Returns blank, this cannot be used
        return "";
        #endif
    }

}
