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

#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "Config.hpp"

#if defined(PLATFORM_UWP)
#include <ppltasks.h>
#endif

namespace ngine {
    /**
     * Log writer class.
     *
     * One day this might even write to a file :O
     */
    class NEAPI Console {
        template <typename... Args>
        static std::string _getFormattedString(std::string format,
                                               Args... args) {
            // Get the size
            auto size = snprintf(nullptr, 0, format.c_str(), args...);

            // Get the string
            std::string output(size + 1, '\0');
#if defined(WIN32)
            sprintf_s(&output[0], size + 1, format.c_str(), args...);
#else
            sprintf(&output[0], format.c_str(), args...);
#endif

            return output;
        }

        /**
         * Write output to console.
         *
         * @todo Write to log file if enabled.
         */
        static void _write(std::string severity, std::string moduleName,
                           std::string msg) {
            std::cout << "[" << severity << "] (" << moduleName << "): " << msg
                      << std::endl;
        }

        static void _showPopup(const std::string &msg,
                               const std::string &title) {
            // TODO: Support more platforms or even make a universal API for
            // message boxes.
#if defined(_WIN32) && defined(PLATFORM_DESKTOP)
            MessageBoxA(NULL, msg.c_str(), title.c_str(), MB_ICONERROR | MB_OK);
#elif defined(PLATFORM_UWP)
            // Create the message dialog and set its content
            // std::wstring tmpMsg(msg.begin(), msg.end());
            // std::wstring tmpTitle(title.begin(), title.end());
            // auto msg = ref new Windows::UI::Popups::MessageDialog(ref new
            // Platform::String(tmpMsg.c_str()), ref new
            // Platform::String(tmpTitle.c_str()));
            // TODO: Sync show
#endif
        }

    public:
        /**
         * Write a notice to the console.
         *
         * @tparam Args Parameter pack types.
         * @param moduleName The module.
         * @param format The string format.
         * @param args Anything extra.
         */
        template <typename... Args>
        static void notice(std::string moduleName, std::string format,
                           Args... args) {
            _write("NOTICE", moduleName, _getFormattedString(format, args...));
        }

        /**
         * Write a warning to the console.
         *
         * @tparam Args Parameter pack types.
         * @param moduleName The module.
         * @param format The string format.
         * @param args Anything extra.
         */
        template <typename... Args>
        static void warn(std::string moduleName, std::string format,
                         Args... args) {
            _write("WARN", moduleName, _getFormattedString(format, args...));
        }

        /**
         * Write an error to the console.
         *
         * @todo Allow throwing if error occurs
         * @tparam Args Parameter pack types.
         * @param moduleName The module.
         * @param format The string format.
         * @param args Anything extra.
         */
        template <typename... Args>
        static void error(std::string moduleName, std::string format,
                          Args... args) {
            _write("ERROR", moduleName, _getFormattedString(format, args...));
        }

        /**
         * Write a fatal error to the console, then throw std::runtime_error
         * with the error message.
         *
         * @tparam Args Parameter pack types.
         * @param moduleName The module.
         * @param format The string format.
         * @param args Anything extra.
         */
        template <typename... Args>
        static void fail(std::string moduleName, std::string format,
                         Args... args) {
            auto msg = _getFormattedString(format, args...);
            _write("FATAL", moduleName, msg);
            _showPopup(msg, "Fatal error occurred in \"" + moduleName +
                                "\" module.");
            throw std::runtime_error(msg); // TODO: Exception type tparam?
            exit(-1);
        }
    };
} // namespace ngine

#endif // CONSOLE_HPP
