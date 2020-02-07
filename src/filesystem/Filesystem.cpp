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

#include "filesystem/Filesystem.hpp"

#if defined(_WIN32)
#include <Windows.h>
#include <shlobj.h>
#elif defined(__linux__)
#define _GNU_SOURCE
#define __USE_GNU
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <linux/limits.h>
#include <dlfcn.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <dlfcn.h>
#include <sys/syslimits.h>
#include <unistd.h>
#include <pwd.h>
#endif

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <cstring>
#include <sstream>

#include "Console.hpp"

namespace ngine::filesystem {
    ////////
    // Path
    ////////

    // Public Constructors

    Path::Path() {
        // Mark as improper
        m_hasProperValue = false;
    }

    Path::Path(const std::string &pathString) {
        // Set internal path
        m_internalPath = pathString;

        // Check path
        _correctPath();
    }

    Path::Path(const std::string &path, const std::string &pathB) {
        // Set internal path
        m_internalPath = path + _getJoinChar() + pathB;

        // Check path
        _correctPath();
    }

    Path::Path(const Path& path, const Path& pathB) {
        // Set internal path
        m_internalPath = path.m_internalPath + _getJoinChar() + pathB.m_internalPath;

        // Check path
        _correctPath();
    }

    Path::Path(const Path &path, const std::string &pathB) {
        // Set internal path
        m_internalPath = path.m_internalPath + _getJoinChar() + pathB;

        // Check path
        _correctPath();
    }

    Path::Path(const std::string &path, const Path &pathB) {
        // Set internal path
        m_internalPath = path + _getJoinChar() + pathB.m_internalPath;

        // Check path
        _correctPath();
    }

    // Public Methods

    Path Path::getAbsolute() const {
        // If we are already absolute, ignore
        if (isAbsolute()) return *this;

        // Get relative to executable dir
        return GetExecutableDirectory() / *this;
    }

    Path Path::GetAppDataDirectory() {
#if defined(PLATFORM_DESKTOP)
#if defined(_WIN32)
        // %APPDATA%

        return std::string(getenv("APPDATA"));
#elif defined(__linux__)
        // Home local share
        return _getHome() + ".local/share";
#elif defined(__APPLE__)
        // Application Support
        return __GetHome() + "/Library/Application Support"
#endif
#elif defined(PLATFORM_UWP)
        // Windows specific path
        auto roamingAppData = Windows::Storage::ApplicationData::Current->RoamingFolder->Path->ToString();
        std::wstring tmp(roamingAppData->Begin());
        std::string path(tmp.begin(), tmp.end());
        return path;
#endif
    }

    Path Path::GetExecutableDirectory() {
#if defined(PLATFORM_UWP)
        auto installed = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
        std::wstring tmp(installed->Begin());
        std::string installedPath(tmp.begin(), tmp.end());

        return installedPath;
#else
        return GetExecutablePath().getParent();
#endif
    }

    Path Path::GetExecutablePath() {
        // https://github.com/cginternals/cpplocate/blob/master/source/liblocate/source/liblocate.c#L39
        // I trust this works as there are no issues about it...
#if defined(_WIN32) && defined(PLATFORM_DESKTOP)
        char exePath[MAX_PATH];
        unsigned int len = GetModuleFileNameA(GetModuleHandleA(nullptr), exePath, MAX_PATH);
        if (len == 0)
            return Path("");

        return Path(std::string(exePath));
#elif defined(_WIN32) && defined(PLATFORM_UWP)
        throw std::runtime_error("Cannot get executable path of UWP app, get executable directory instead.");
#elif defined(__linux__)
        char exePath[PATH_MAX];
        int len = readlink("/proc/self/exe", exePath, PATH_MAX);
        if (len <= 0 || len == PATH_MAX)
            return Path("");

        return Path(std::string(exePath));
#elif defined(__APPLE__)
        char exePath[PATH_MAX];

        unsigned int len = (unsigned int)PATH_MAX;

        if (_NSGetExecutablePath(exePath, &len) == 0)
        {
            char * realPath = realpath(exePath, 0x0);

            if (realPath == 0x0)
            {
                return Path("");
            }

            auto pathStr = std::string(realPath);

            free(realPath);

            return Path(pathStr);
        }
        else
        {
            char * intermediatePath = (char *)malloc(sizeof(char) * len);

            if (_NSGetExecutablePath(intermediatePath, &len) != 0)
            {
                free(intermediatePath);
                return Path("");
            }

            char * realPath = realpath(intermediatePath, 0x0);

            free(intermediatePath);

            if (realPath == 0x0)
            {
                return Path("");
            }

            auto pathStr = std::string(realPath);

            free(realPath);

            return Path(pathStr);
        }

        #else
        // Returns blank, this cannot be used
        return Path("");
#endif
    }

    std::string Path::getFileExtension() const {
        // Get path string
        auto path = getString();

        // Get file extension
        auto dot = path.find_last_of('.');
        auto lastSlash = path.find_last_of(_getJoinChar());

        if (std::string::npos != dot) {
            if (dot > lastSlash) {
                return path.substr(dot + 1);
            }
        }

        return "";
    }

    std::string Path::getObjectName() const {
        // Get path string
        auto nameTemp = getString();

        // Search for the last directory slash
        auto fSlash = nameTemp.find_last_of(_getJoinChar());
        if (std::string::npos != fSlash) {
            nameTemp.erase(0, fSlash + 1);
        }

        // Remove the file extension
        auto dot = nameTemp.find_last_of('.');
        if (std::string::npos != dot) {
            nameTemp.erase(dot);
        }

        return nameTemp;
    }

    Path Path::getParent() const {
        auto lastSlash = m_internalPath.find_last_of(_getJoinChar());

        if (std::string::npos != lastSlash)
            return Path(m_internalPath.substr(0, lastSlash));
        else return Path();
    }

    Path Path::getRelativeTo(const Path &base) const {
        // The base must be absolute
        auto basePath = base;
        if (!basePath.isAbsolute()) basePath = basePath.getAbsolute();

        // Base must be a directory
        if (basePath.getResourceType() != ResourceType::Directory) throw std::runtime_error("Base must be a directory.");

        // I must be absolute
        if (!isAbsolute()) throw std::runtime_error("Path must be absolute.");

        // Find common path
        auto commonPathPos = 0;

        auto baseStr = basePath.getString();
        auto str = getString();

        for (auto i = 0; i < baseStr.size() && i < str.size(); i++) {
            if (baseStr[i] != str[i]) {
                break;
            }
            commonPathPos++;
        }

        // Catches things like different drives
        if (commonPathPos == 0) {
            throw std::runtime_error("Cannot get relative path to files on different drives.");
        }

        // Remove from both strings
        baseStr.erase(0, commonPathPos);
        str.erase(0, commonPathPos);

        // Remove initial slash if left
        if (str.size() > 0) {
            if (str[0] == _getJoinChar()) {
                str.erase(0, 1);
            }
        }

        // Prepend ../ for all extra parts
        int subdirs = (int)std::count(baseStr.begin(), baseStr.end(), _getJoinChar());

        // Add one more if it doesn't end in a slash
        if (baseStr.size() > 0) {
            if (baseStr[baseStr.size() - 1] != _getJoinChar()) {
                subdirs += 1;
            }
        }

        for (auto i = 0; i < subdirs; i++) {
            str = std::string("..") + _getJoinChar() + str;
        }

        // Return, we're done
        return str;
    }

    ResourceType Path::getResourceType() const {
#if defined(_WIN32)
        DWORD dwAttrib = GetFileAttributesA(getString().c_str());

        if (dwAttrib != INVALID_FILE_ATTRIBUTES) {
            return dwAttrib & FILE_ATTRIBUTE_DIRECTORY ? ResourceType::Directory : ResourceType::File;
        }
#elif defined(__linux__) || defined(__APPLE__)
        // Get path info
        struct stat path_stat;
        stat(getString().c_str(), &path_stat);

        if (S_ISREG(path_stat.st_mode))
            return ResourceType::File;
        else if (S_ISDIR(path_stat.st_mode))
            return ResourceType::Directory;
#endif
        return ResourceType::Invalid;
    }

    std::string Path::getString() const {
        return m_internalPath;
    }

    std::string Path::getStringNoExtension() const {
        auto lastDot = m_internalPath.find_last_of('.');
        auto lastSlash = m_internalPath.find_last_of(_getJoinChar());

        if (std::string::npos != lastDot) {
            if (std::string::npos != lastSlash) {
                if (lastDot > lastSlash) {
                    return m_internalPath.substr(0, lastDot);
                }
            } else {
                return m_internalPath.substr(0, lastDot);
            }
        }

        return m_internalPath;
    }

    Path Path::GetWorkingDirectory() {
#if defined(_WIN32)
        // Create buffer
        DWORD bufferLen = MAX_PATH;
        auto buffer = new char[MAX_PATH + 1];

        // Get current dir
        GetCurrentDirectoryA(MAX_PATH + 1, buffer);

        // Null terminate
        buffer[MAX_PATH] = 0;

        // Convert to string
        std::string string(buffer);

        // Delete buffer
        delete[] buffer;

        // Return
        return Path(string);
#elif defined(__linux) || defined(__APPLE__)
        // Create buffer
        auto buffer = new char[PATH_MAX];

        // Get working dir
        if (getcwd(buffer, PATH_MAX) == nullptr) throw std::runtime_error("Unable to determine working directory.");

        // Convert to string
        std::string string(buffer);

        // Delete buffer
        delete[] buffer;

        // Return
        return Path(string);
#endif
    }

    bool Path::isAbsolute() const {
#if defined(_WIN32)
        // Test if we have (*):\ at the start
        if (m_internalPath.size() > 3)
            return m_internalPath[1] == ':' && m_internalPath[2] == '\\';
#elif defined(__linux__) || defined(__APPLE__)
        // Test we have an initial slash
        if (m_internalPath.size() > 0)
            return m_internalPath[0] == '/';
#endif
        return false;
    }

    Path Path::Join(const std::string &pathA, const std::string &pathB) {
        return Path(pathA, pathB);
    }

    Path Path::Join(const Path &pathA, const Path &pathB) {
        return Path(pathA, pathB);
    }

    bool Path::isValid() const {
        return m_hasProperValue;
    }

    // Operators

    Path operator/(const std::string &path, const std::string &pathB) {
        return Path(path, pathB);
    }

    Path operator/(const Path &path, const Path &pathB) {
        return Path(path, pathB);
    }

    Path operator/(const std::string &path, const Path &pathB) {
        return Path(path, pathB);
    }

    Path operator/(const Path &path, const std::string &pathB) {
        return Path(path, pathB);
    }

    void Path::operator/=(const Path &pathB) {
        // Set internal path
        m_internalPath = m_internalPath + _getJoinChar() + pathB.m_internalPath;

        // Check path
        _correctPath();
    }

    void Path::operator/=(const std::string &pathB) {
        // Set internal path
        m_internalPath = m_internalPath + _getJoinChar() + pathB;

        // Check path
        _correctPath();
    }

    Path::operator std::string() const {
        return m_internalPath;
    }

    bool Path::operator==(Path rhs) {
        return getAbsolute() == rhs.getAbsolute();
    }

    bool Path::operator!=(Path rhs) {
        return getAbsolute() != rhs.getAbsolute();
    }

    // Private Methods

    std::string Path::_cleanPathString(const std::string &str) {
#if defined(_WIN32) && defined(PLATFORM_DESKTOP)
        // TODO: Another way to fix Unicode path names.
        // TODO: Basically Ngine needs better Unicode support in general
        return str;
//        // Get path length
//        auto len = GetShortPathNameA(str_.c_str(), nullptr, 0);
//
//        // Check length
//        if (len == 0) {
//            auto e = GetLastError();
//            if (e == 0x2 || e == 0x3) // File not found/Path not found, cannot clean it
//                return str_;
//            else throw std::runtime_error("GetShortPathNameA error.");
//        }
//
//        // Allocate buffer
//        auto buffer = new char[len];
//
//        // Get path
//        len = GetShortPathNameA(str_.c_str(), buffer, len);
//
//        // Check length
//        if (len == 0) {
//            auto e = GetLastError();
//            if (e == 0x2 || e == 0x3) // File not found/Path not found, cannot clean it
//                return str_;
//            else throw std::runtime_error("GetShortPathNameA error.");
//        }
//
//        // Convert to string
//        auto string = std::string(buffer);
//
//        // Delete buffer
//        delete[] buffer;
//
//        return string;
#endif
        return str;
    }

    void Path::_correctPath() {
        // Clean path
        m_internalPath = _cleanPathString(m_internalPath);

        // Search for empty string
        if (m_internalPath.empty()) {
            // Not a correct value
            m_hasProperValue = false;
            return;
        }

        // Look for any non-whitespace chars
        bool hasNonWhitespace = false;
        for (auto c : m_internalPath) {
            if (c != ' ') {
                hasNonWhitespace = true;
                break;
            }
        }

        if (!hasNonWhitespace) {
            // Not a correct value
            m_hasProperValue = false;
            return;
        }

        // Look for notation foreign to this OS
        if (_getJoinChar() != '\\') {
            std::replace(m_internalPath.begin(), m_internalPath.end(), '\\', _getJoinChar());
        }

        if (_getJoinChar() != '/') {
            std::replace(m_internalPath.begin(), m_internalPath.end(), '/', _getJoinChar());
        }

        // Is valid
        m_hasProperValue = true;
    }

    std::string Path::_getHome() {
#if defined(__linux__) || defined(__APPLE__)
        int uid = getuid();

        // Use HOME environment variable if not run as root
        const char *homeVar = std::getenv("HOME");
        if (uid != 0 && homeVar) return std::string(homeVar);

        // Use psswd home
        struct passwd *pw = getpwuid(uid);
        return std::string(pw->pw_dir);
#endif
        return "";
    }

    char Path::_getJoinChar() {
#if defined(_WIN32)
        return '\\';
#else
        return '/';
#endif
    }

    ////////
    // FilesystemObject
    ////////

    // Public Methods

    void FilesystemObject::move(const Path &newPath) {
        // Move file
        ::rename(m_objectPath.getString().c_str(), newPath.getString().c_str());
    }

    void FilesystemObject::rename(const std::string &newName) {
        // Rename
        move(m_objectPath / newName);
    }

    std::string FilesystemObject::getName() const {
        return m_objectPath.getObjectName();
    }

    Path FilesystemObject::getPath() const {
        return m_objectPath;
    }

    // Protected Constructors

    FilesystemObject::FilesystemObject(const Path &path) : m_objectPath(path) {}

    ////////
    // File
    ////////

    // InternalFileHandler Destructor

    File::InternalFileHandler::~InternalFileHandler() {
        if (InternalHandle != nullptr)
            fclose(InternalHandle);
        InternalHandle = nullptr;
    }

    // Public Constructor(s)

    File::File() : FilesystemObject(Path()) {
        // Create an empty handler
        m_internalHandle = std::make_shared<InternalFileHandler>();
    }

    File::File(const Path &path) : FilesystemObject(path) {
        // Check path is valid
        if (!path.isValid()) throw std::runtime_error("File must be given a valid path.");

        // Create an empty handler
        m_internalHandle = std::make_shared<InternalFileHandler>();
    }

    // Destructor

    File::~File() {
        // Close file in case it isnt gone already
        close();
    }

    // Public Methods

    void File::close() {
        // Close file
        if (m_internalHandle->InternalHandle != nullptr) {
            fclose(m_internalHandle->InternalHandle);
            m_internalHandle->InternalHandle = nullptr;
        }

        // Set mode
        m_internalOpenMode = FileOpenMode::Closed;
    }

    File File::CreateNewFile(const Path &path, bool leaveOpen, bool binary) {
        File f(path);
        f.open(FileOpenMode::Write, binary);
        if (!leaveOpen)
            f.close();
        return f;
    }

    bool File::deleteObject() {
        // Ensure that we are closed
        close();

        // Remove from filesystem
        return remove(m_objectPath.getString().c_str()) == 0;
    }

    bool File::exists() const {
        // If we are open, we know we exist
        if (isOpen()) return true;

        // Using C apis so that it is cross platform
        FILE *file = fopen(m_objectPath.getString().c_str(), "r");
        if (file != nullptr) {
            fclose(file);
            return true;
        }
        return false;
    }

    FileOpenMode File::getCurrentOpenMode() const {
        return m_internalOpenMode;
    }

    File File::GetFile(const Path &path) {
        return File(path);
    }

    std::string File::getFileExtension() const {
        return m_objectPath.getFileExtension();
    }

    FILE *File::getFileHandle() const {
        if (!isOpen()) {
            Console::Warn("File", "Cannot get handle of file that is not open.");
            return nullptr;
        }

        return m_internalHandle->InternalHandle;
    }

    int File::getSize() const {
        if (!isOpen()) {
            Console::Warn("File", "Cannot determine size of file that is not open.");
            return 0;
        }

        fseek(m_internalHandle->InternalHandle, 0, SEEK_END);
        auto s = ftell(m_internalHandle->InternalHandle);
        fseek(m_internalHandle->InternalHandle, 0, SEEK_SET);

        return s;
    }

    bool File::isOpen() const {
        if (m_internalHandle == nullptr) return false;

        return m_internalHandle->InternalHandle != nullptr;
    }

    bool File::open(FileOpenMode mode, bool binary) {
        // Check validity of path
        if (!m_objectPath.isValid()) throw std::runtime_error("This file's path is invalid");

        // Open with selected mode
        switch(mode) {
            case FileOpenMode::Read:
                // Check this is actually a file
                if (m_objectPath.getResourceType() != ResourceType::File) throw std::runtime_error("This path does not point to a file.");

                // Open binary file for read
                m_internalHandle->InternalHandle = fopen(m_objectPath.getString().c_str(), binary ? "rb" : "r");

                // Set mode
                m_internalOpenMode = mode;
                break;
            case FileOpenMode::Write:
                // Open binary file for write
                m_internalHandle->InternalHandle = fopen(m_objectPath.getString().c_str(), binary ? "wb" : "w");

                // Set mode
                m_internalOpenMode = mode;
                break;
            case FileOpenMode::Append:
                // Open binary file for append
                m_internalHandle->InternalHandle = fopen(m_objectPath.getString().c_str(), binary ? "ab" : "a");

                // Set mode
                m_internalOpenMode = mode;
                break;
            case FileOpenMode::ReadWrite:
                // Open binary file for read and write
                m_internalHandle->InternalHandle = fopen(m_objectPath.getString().c_str(), binary ? "w+b" : "w+");

                // Set mode
                m_internalOpenMode = mode;
                break;
            case FileOpenMode::ReadAppend:
                // Open binary file for read and append
                m_internalHandle->InternalHandle = fopen(m_objectPath.getString().c_str(), binary ? "a+b" : "a+");

                // Set mode
                m_internalOpenMode = mode;
                break;
            default:
                Console::Warn("File", "File mode not supported.");

                // Set mode
                m_internalOpenMode = FileOpenMode::Closed;
                break;
        }

        // Return success
        return isOpen();
    }

    unsigned char *File::readBytes(int size, int offset) {
        if (!isOpen()) throw std::runtime_error("Cannot read from closed file.");

        // Check for our mode
        if (m_internalOpenMode != FileOpenMode::Read
            && m_internalOpenMode != FileOpenMode::ReadWrite
            && m_internalOpenMode != FileOpenMode::ReadAppend)
            throw std::runtime_error("File not opened for reading.");

        // Determine file size if size is -1
        auto filesize_ = getSize();

        if (size == -1) {
            size = filesize_;
        }

        if (size <= 0) {
            throw std::runtime_error("Invalid read size.");
        }

        if (offset >= filesize_ || offset < 0) {
            throw std::runtime_error("Invalid offset.");
        }

        if (size + offset > filesize_) {
            throw std::runtime_error("Data out of bounds.");
        }

        // Seek to the offset
        fseek(m_internalHandle->InternalHandle, offset, SEEK_SET);

        // Read bytes to array
        auto *buffer = new unsigned char[size];
        fread(buffer, size, 1, m_internalHandle->InternalHandle);

        // Return data
        return buffer;
    }

    std::string File::readString(int size, int offset) {
        // Check we're open
        if (!isOpen()) throw std::runtime_error("Cannot read from closed file.");

        // Check for our mode
        if (m_internalOpenMode != FileOpenMode::Read
            && m_internalOpenMode != FileOpenMode::ReadWrite
            && m_internalOpenMode != FileOpenMode::ReadAppend)
            throw std::runtime_error("File not opened for reading.");

        // Determine file size if size is -1
        auto filesize_ = getSize();

        if (size == -1) {
            size = filesize_;
        }

        if (size <= 0) {
            throw std::runtime_error("Invalid read size.");
        }

        if (offset >= filesize_ || offset < 0) {
            throw std::runtime_error("Invalid offset.");
        }

        if (size + offset > filesize_) {
            throw std::runtime_error("Data out of bounds.");
        }

        // Seek to the offset
        fseek(m_internalHandle->InternalHandle, offset, SEEK_SET);

        // Read to c string
        auto buffer = new char[size + 1];
        int r = fread(buffer, 1, size, m_internalHandle->InternalHandle);

        // Null-terminate buffer
        buffer[r] = '\0';

        // Convert to string
        auto str = std::string(buffer);

        // Delete buffer
        delete[] buffer;

        return str;
    }

    bool File::writeBytes(unsigned char *data, int size) {
        // Check we're open
        if (!isOpen()) throw std::runtime_error("Cannot write to a closed file.");

        // Check for our mode
        if (m_internalOpenMode != FileOpenMode::Write
            && m_internalOpenMode != FileOpenMode::Append
            && m_internalOpenMode != FileOpenMode::ReadWrite
            && m_internalOpenMode != FileOpenMode::ReadAppend)
            throw std::runtime_error("File not opened for writing.");

        // Write
        return fwrite(data, 1, size, m_internalHandle->InternalHandle) == 1;
    }

    bool File::writeString(const std::string &string) {
        // Check we're open
        if (!isOpen()) throw std::runtime_error("Cannot write to closed file.");

        // Check for our mode
        if (m_internalOpenMode != FileOpenMode::Write
            && m_internalOpenMode != FileOpenMode::Append
            && m_internalOpenMode != FileOpenMode::ReadWrite
            && m_internalOpenMode != FileOpenMode::ReadAppend)
            throw std::runtime_error("File not opened for writing.");

        // Write string
        return fputs(string.c_str(), m_internalHandle->InternalHandle) != EOF;
    }

    ////////
    // Directory
    ////////

    Directory::Directory() : FilesystemObject(Path()) {}

    Directory::Directory(const Path &path) : FilesystemObject(path) {
        // Check for valid path
        if (!path.isValid()) throw std::runtime_error("Directory must be given a valid path.");
    }

    bool Directory::create() {
        auto success = false;
#if defined(_WIN32)
        // Create directory
        success = CreateDirectoryA(getPath().getString().c_str(), nullptr) != 0;
#elif defined(__linux__) || defined(__APPLE__)
        // Create directory
        success = mkdir(getPath().getString().c_str(), 0777) == 0;
#endif
        return success;
    }

    std::pair<bool, Directory> Directory::Create(const Path &path) {
        auto dir = Directory(path);
        return {dir.create(), dir};
    }

    bool Directory::deleteObject() {
        // Check
        _throwAccessErrors();

#if defined(_WIN32)
        // Try to delete (not recursive)
        auto del = RemoveDirectoryA(m_objectPath.getString().c_str());
        return del != 0;
#elif defined(__linux__) || defined(__APPLE__)
        return remove(m_objectPath.getString().c_str()) == 0;
#endif
        return false;
    }

    bool Directory::deleteRecursive() {
        // Check
        _throwAccessErrors();

        // Success tracker
        auto success = true;

        // Delete my own files
        for (auto file : getFiles()) {
            if (!file.deleteObject()) {
                success = false;
                break;
            }
        }

        // Stop if we find an issue
        if (!success)
            return false;

        // Get directories
        auto dirs = getDirectories();

        // Delete child directories
        for (auto dir : dirs) {
            if (!dir.deleteRecursive()) {
                success = false;
                break;
            }
        }

        // Stop if we find an issue
        if (!success)
            return false;


        // Delete self
        success = deleteObject();

        // Return
        return success;
    }

    bool Directory::exists() const {
#if defined(_WIN32)
        // https://stackoverflow.com/a/6218445
        // Get attributes for directory
        DWORD dwAttrib = GetFileAttributesA(m_objectPath.getString().c_str());

        return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
                (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#elif defined(__linux__) || defined(__APPLE__)
        // Test opening of file
        DIR *dir = opendir(m_objectPath.getString().c_str());
        if (dir) {
            closedir(dir);
            return true;
        }
        return false;
#endif
        return false;
    }

    Directory Directory::getAppDataDirectory() {
        return Directory(Path::GetAppDataDirectory());
    }

    std::vector<Directory> Directory::getDirectories() const {
        // Check
        _throwAccessErrors();

        // Directories vector
        auto dirs = std::vector<Directory>();
#if defined(_WIN32)
        // Find first directory
        WIN32_FIND_DATAA FindFileData;
        HANDLE hFind = FindFirstFileA((m_objectPath.getString() + "\\*").c_str(), &FindFileData);

        // Check exists
        if (hFind == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Invalid directory.");
        }

        // Search for directories
        do {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                auto dirName = FindFileData.cFileName;
                // Avoids . and .. directories
                if (strcmp(dirName, ".") != 0 && strcmp(dirName, "..") != 0)
                    dirs.push_back(Directory(Path(m_objectPath, dirName)));
            }
        } while (FindNextFileA(hFind, &FindFileData) != 0);

        // Close search
        FindClose(hFind);
#elif defined(__linux__) || defined(__APPLE__)
        // Variables
        DIR *dir;
        dirent *entry;

        // Open dir
        dir = opendir(m_objectPath.getString().c_str());

        // Test open
        if (!dir) throw std::runtime_error("Cannot open directory.");

        // Read all directories
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                    dirs.push_back(Directory(Path(m_objectPath, entry->d_name)));
            }
        }

        // Close directory
        closedir(dir);
#endif
        return dirs;
    }

    std::vector<File> Directory::getFiles() const {
        // Check
        _throwAccessErrors();

        // Files vector
        auto files = std::vector<File>();
#if defined(_WIN32)
        // Find the first file in the directory
        WIN32_FIND_DATAA FindFileData;
        HANDLE hFind = FindFirstFileA((m_objectPath.getString() + "\\*").c_str(), &FindFileData);

        // Check it exists
        if (hFind == INVALID_HANDLE_VALUE) {
            auto err = GetLastError();
            throw std::runtime_error("Invalid directory.");
        }

        // Get all files
        do {
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                auto filename = FindFileData.cFileName;
                files.push_back(File(Path(m_objectPath, filename)));
            }
        } while (FindNextFileA(hFind, &FindFileData) != 0);

        // Close search
        FindClose(hFind);
#elif defined(__linux__) || defined(__APPLE__)
        // Variables
        DIR *dir;
        dirent *entry;

        // Open dir
        dir = opendir(m_objectPath.getString().c_str());

        // Test open
        if (!dir) throw std::runtime_error("Cannot open directory.");

        // Read all directories
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type != DT_DIR) {
                files.push_back(File(Path(m_objectPath, entry->d_name)));
            }
        }

        // Close directory
        closedir(dir);
#endif
        return files;
    }

    std::vector<File> Directory::getFilesRecursive() const {
        // Check
        _throwAccessErrors();

        // Keep track of all files
        auto files = std::vector<File>();

        // Grab my files
        auto myFiles = getFiles();
        files.insert(files.end(), myFiles.begin(), myFiles.end());

        // Get all descendant directories
        auto dirs = getDirectories();

        for (auto dir : dirs) {
            auto dirFiles = dir.getFilesRecursive();
            files.insert(files.end(), dirFiles.begin(), dirFiles.end());
        }

        return files;
    }

    Directory Directory::GetDirectory(const Path &path) {
        return Directory(path);
    }

    Directory Directory::GetExecutableDirectory() {
        return Directory(Path::GetExecutableDirectory());
    }

    Directory Directory::GetWorkingDirectory() {
        return Directory(Path::GetWorkingDirectory());
    }

    // Private Fields

    void Directory::_throwAccessErrors() const {
        // Check exists
        if (!exists()) throw std::runtime_error("This directory does not exist.");

        // Check this is actually a directory
        if (getPath().getResourceType() != ResourceType::Directory) throw std::runtime_error("This path does not point to a directory.");
    }
}
