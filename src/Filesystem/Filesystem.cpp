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

#include "Filesystem.h"

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

#include <sstream>

namespace NerdThings::Ngine::Filesystem {
    ////////
    // Path
    ////////

    // Public Constructors

    Path::Path() {
        // Mark as improper
        _HasProperValue = false;
    }

    Path::Path(const std::string &pathString_) {
        // Set internal path
        _InternalPath = pathString_;

        // Check path
        __CorrectPath();
    }

    Path::Path(const std::string &path_, const std::string &pathB_) {
        // Set internal path
        _InternalPath = path_ + __GetJoinChar() + pathB_;

        // Check path
        __CorrectPath();
    }

    Path::Path(const Path& path_, const Path& pathB_) {
        // Set internal path
        _InternalPath = path_._InternalPath + __GetJoinChar() + pathB_._InternalPath;

        // Check path
        __CorrectPath();
    }

    Path::Path(const Path &path_, const std::string &pathB_) {
        // Set internal path
        _InternalPath = path_._InternalPath + __GetJoinChar() + pathB_;

        // Check path
        __CorrectPath();
    }

    Path::Path(const std::string &path_, const Path &pathB_) {
        // Set internal path
        _InternalPath = path_ + __GetJoinChar() + pathB_._InternalPath;

        // Check path
        __CorrectPath();
    }

    // Public Methods

    Path Path::GetAbsolute() const {
        // If we are already absolute, ignore
        if (IsAbsolute()) return *this;

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
        return __GetHome() + ".local/share";
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
        return GetExecutablePath().GetParent();
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

    std::string Path::GetFileExtension() const {
        // Get path string
        auto path = GetString();

        // Get file extension
        auto dot = path.find_last_of('.');
        auto lastSlash = path.find_last_of(__GetJoinChar());

        if (std::string::npos != dot) {
            if (dot > lastSlash) {
                return path.substr(dot + 1);
            }
        }

        return "";
    }

    std::string Path::GetObjectName() const {
        // Get path string
        auto nameTemp = GetString();

        // Search for the last directory slash
        auto fSlash = nameTemp.find_last_of(__GetJoinChar());
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

    Path Path::GetParent() const {
        auto lastSlash = _InternalPath.find_last_of(__GetJoinChar());

        if (std::string::npos != lastSlash)
            return Path(_InternalPath.substr(0, lastSlash));
        else return Path();
    }

    Path Path::GetRelativeTo(const Path &base_) const {
        // The base must be absolute
        auto basePath = base_;
        if (!basePath.IsAbsolute()) basePath = basePath.GetAbsolute();

        // Base must be a directory
        if (basePath.GetResourceType() != TYPE_DIRECTORY) throw std::runtime_error("Base must be a directory.");

        // I must be absolute
        if (!IsAbsolute()) throw std::runtime_error("Path must be absolute.");

        // Find common path
        auto commonPathPos = 0;

        auto baseStr = basePath.GetString();
        auto str = GetString();

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
            if (str[0] == __GetJoinChar()) {
                str.erase(0, 1);
            }
        }

        // Prepend ../ for all extra parts
        int subdirs = (int)std::count(baseStr.begin(), baseStr.end(), __GetJoinChar());

        // Add one more if it doesn't end in a slash
        if (baseStr.size() > 0) {
            if (baseStr[baseStr.size() - 1] != __GetJoinChar()) {
                subdirs += 1;
            }
        }

        for (auto i = 0; i < subdirs; i++) {
            str = std::string("..") + __GetJoinChar() + str;
        }

        // Return, we're done
        return str;
    }

    ResourceType Path::GetResourceType() const {
#if defined(_WIN32)
        DWORD dwAttrib = GetFileAttributesA(GetString().c_str());

        if (dwAttrib != INVALID_FILE_ATTRIBUTES) {
            return dwAttrib & FILE_ATTRIBUTE_DIRECTORY ? TYPE_DIRECTORY : TYPE_FILE;
        }
#elif defined(__linux__) || defined(__APPLE__)
        // Get path info
        struct stat path_stat;
        stat(GetString().c_str(), &path_stat);

        if (S_ISREG(path_stat.st_mode))
            return TYPE_FILE;
        else if (S_ISDIR(path_stat.st_mode))
            return TYPE_DIRECTORY;
#endif
        return TYPE_INVALID;
    }

    std::string Path::GetString() const {
        return _InternalPath;
    }

    std::string Path::GetStringNoExtension() const {
        auto lastDot = _InternalPath.find_last_of('.');
        auto lastSlash = _InternalPath.find_last_of(__GetJoinChar());

        if (std::string::npos != lastDot) {
            if (std::string::npos != lastSlash) {
                if (lastDot > lastSlash) {
                    return _InternalPath.substr(0, lastDot);
                }
            } else {
                return _InternalPath.substr(0, lastDot);
            }
        }

        return _InternalPath;
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

    bool Path::IsAbsolute() const {
#if defined(_WIN32)
        // Test if we have (*):\ at the start
        if (_InternalPath.size() > 3)
            return _InternalPath[1] == ':' && _InternalPath[2] == '\\';
#elif defined(__linux__) || defined(__APPLE__)
        // Test we have an initial slash
        if (_InternalPath.size() > 0)
            return _InternalPath[0] == '/';
#endif
        return false;
    }

    Path Path::Join(const std::string &pathA_, const std::string &pathB_) {
        return Path(pathA_, pathB_);
    }

    Path Path::Join(const Path &pathA_, const Path &pathB_) {
        return Path(pathA_, pathB_);
    }

    bool Path::Valid() const {
        return _HasProperValue;
    }

    // Operators

    Path operator/(const std::string &path_, const std::string &pathB_) {
        return Path(path_, pathB_);
    }

    Path operator/(const Path &path_, const Path &pathB_) {
        return Path(path_, pathB_);
    }

    Path operator/(const std::string &path_, const Path &pathB_) {
        return Path(path_, pathB_);
    }

    Path operator/(const Path &path_, const std::string &pathB_) {
        return Path(path_, pathB_);
    }

    void Path::operator/=(const Path &pathB_) {
        // Set internal path
        _InternalPath = _InternalPath + __GetJoinChar() + pathB_._InternalPath;

        // Check path
        __CorrectPath();
    }

    void Path::operator/=(const std::string &pathB_) {
        // Set internal path
        _InternalPath = _InternalPath + __GetJoinChar() + pathB_;

        // Check path
        __CorrectPath();
    }

    Path::operator std::string() const {
        return _InternalPath;
    }

    bool Path::operator==(Path rhs) {
        return GetAbsolute() == rhs.GetAbsolute();
    }

    bool Path::operator!=(Path rhs) {
        return GetAbsolute() != rhs.GetAbsolute();
    }

    // Private Methods

    std::string Path::__CleanPathString(const std::string &str_) {
#if defined(_WIN32) && defined(PLATFORM_DESKTOP)
        // TODO: Another way to fix Unicode path names.
        // TODO: Basically Ngine needs better Unicode support in general
        return str_;
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
        return str_;
    }

    void Path::__CorrectPath() {
        // Clean path
        _InternalPath = __CleanPathString(_InternalPath);

        // Search for empty string
        if (_InternalPath.empty()) {
            // Not a correct value
            _HasProperValue = false;
            return;
        }

        // Look for any non-whitespace chars
        bool hasNonWhitespace = false;
        for (auto c : _InternalPath) {
            if (c != ' ') {
                hasNonWhitespace = true;
                break;
            }
        }

        if (!hasNonWhitespace) {
            // Not a correct value
            _HasProperValue = false;
            return;
        }

        // Look for notation foreign to this OS
        if (__GetJoinChar() != '\\') {
            std::replace(_InternalPath.begin(), _InternalPath.end(), '\\', __GetJoinChar());
        }

        if (__GetJoinChar() != '/') {
            std::replace(_InternalPath.begin(), _InternalPath.end(), '/', __GetJoinChar());
        }

        // Is valid
        _HasProperValue = true;
    }

    std::string Path::__GetHome() {
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

    char Path::__GetJoinChar() {
        // TODO: See if there are any more variations
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

    void FilesystemObject::Move(const Path &newPath_) {
        // Move file
        rename(ObjectPath.GetString().c_str(), newPath_.GetString().c_str());
    }

    void FilesystemObject::Rename(const std::string &newName_) {
        // Rename
        Move(ObjectPath / newName_);
    }

    std::string FilesystemObject::GetObjectName() const {
        return ObjectPath.GetObjectName();
    }

    Path FilesystemObject::GetObjectPath() const {
        return ObjectPath;
    }

    // Protected Constructors

    FilesystemObject::FilesystemObject(const Path &path_) : ObjectPath(path_) {}

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
        _InternalHandle = std::make_shared<InternalFileHandler>();
    }

    File::File(const Path &path_) : FilesystemObject(path_) {
        // Check path is valid
        if (!path_.Valid()) throw std::runtime_error("File must be given a valid path.");

        // Create an empty handler
        _InternalHandle = std::make_shared<InternalFileHandler>();
    }

    // Destructor

    File::~File() {
        // Close file in case it isnt gone already
        Close();
    }

    // Public Methods

    void File::Close() {
        // Close file
        if (_InternalHandle->InternalHandle != nullptr) {
            fclose(_InternalHandle->InternalHandle);
            _InternalHandle->InternalHandle = nullptr;
        }

        // Set mode
        _InternalOpenMode = MODE_NONE;
    }

    File File::CreateNewFile(const Path &path_, bool leaveOpen_, bool binary_) {
        File f(path_);
        f.Open(MODE_WRITE, binary_);
        if (!leaveOpen_)
            f.Close();
        return f;
    }

    bool File::Delete() {
        // Ensure that we are closed
        Close();

        // Remove from filesystem
        return remove(ObjectPath.GetString().c_str()) == 0;
    }

    bool File::Exists() const {
        // If we are open, we know we exist
        if (IsOpen()) return true;

        // Using C apis so that it is cross platform
        FILE *file = fopen(ObjectPath.GetString().c_str(), "r");
        if (file != nullptr) {
            fclose(file);
            return true;
        }
        return false;
    }

    FileOpenMode File::GetCurrentMode() const {
        return _InternalOpenMode;
    }

    File File::GetFile(const Path &path_) {
        return File(path_);
    }

    std::string File::GetFileExtension() const {
        return ObjectPath.GetFileExtension();
    }

    FILE *File::GetFileHandle() const {
        if (!IsOpen()) {
            ConsoleMessage("Cannot get handle of file that is not open.", "WARN", "File");
            return nullptr;
        }

        return _InternalHandle->InternalHandle;
    }

    int File::GetSize() const {
        if (!IsOpen()) {
            ConsoleMessage("Cannot determine size of file that is not open.", "WARN", "File");
            return 0;
        }

        fseek(_InternalHandle->InternalHandle, 0, SEEK_END);
        auto s = ftell(_InternalHandle->InternalHandle);
        fseek(_InternalHandle->InternalHandle, 0, SEEK_SET);

        return s;
    }

    bool File::IsOpen() const {
        if (_InternalHandle == nullptr) return false;

        return _InternalHandle->InternalHandle != nullptr;
    }

    bool File::Open(FileOpenMode mode_, bool binary_) {
        // Check validity of path
        if (!ObjectPath.Valid()) throw std::runtime_error("This file's path is invalid");

        // Open with selected mode
        switch(mode_) {
            case MODE_READ:
                // Check this is actually a file
                if (ObjectPath.GetResourceType() != TYPE_FILE) throw std::runtime_error("This path does not point to a file.");

                // Open binary file for read
                _InternalHandle->InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "rb" : "r");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_WRITE:
                // Open binary file for write
                _InternalHandle->InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "wb" : "w");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_APPEND:
                // Open binary file for append
                _InternalHandle->InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "ab" : "a");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_READ_WRITE:
                // Open binary file for read and write
                _InternalHandle->InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "w+b" : "w+");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_READ_APPEND:
                // Open binary file for read and append
                _InternalHandle->InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "a+b" : "a+");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            default:
                ConsoleMessage("File mode not supported.", "WARN", "File");

                // Set mode
                _InternalOpenMode = MODE_NONE;
                break;
        }

        // Return success
        return IsOpen();
    }

    unsigned char *File::ReadBytes(int size_, int offset_) {
        if (!IsOpen()) throw std::runtime_error("Cannot read from closed file.");

        // Check for our mode
        if (_InternalOpenMode != MODE_READ
            && _InternalOpenMode != MODE_READ_WRITE
            && _InternalOpenMode != MODE_READ_APPEND)
            throw std::runtime_error("File not opened for reading.");

        // Determine file size if size is -1
        auto filesize_ = GetSize();

        if (size_ == -1) {
            size_ = filesize_;
        }

        if (size_ <= 0) {
            throw std::runtime_error("Invalid read size.");
        }

        if (offset_ >= filesize_ || offset_ < 0) {
            throw std::runtime_error("Invalid offset.");
        }

        if (size_ + offset_ > filesize_) {
            throw std::runtime_error("Data out of bounds.");
        }

        // Seek to the offset
        fseek(_InternalHandle->InternalHandle, offset_, SEEK_SET);

        // Read bytes to array
        auto *buffer = new unsigned char[size_];
        fread(buffer, size_, 1, _InternalHandle->InternalHandle);

        // Return data
        return buffer;
    }

    std::string File::ReadString(int size_, int offset_) {
        // Check we're open
        if (!IsOpen()) throw std::runtime_error("Cannot read from closed file.");

        // Check for our mode
        if (_InternalOpenMode != MODE_READ
            && _InternalOpenMode != MODE_READ_WRITE
            && _InternalOpenMode != MODE_READ_APPEND)
            throw std::runtime_error("File not opened for reading.");

        // Determine file size if size is -1
        auto filesize_ = GetSize();

        if (size_ == -1) {
            size_ = filesize_;
        }

        if (size_ <= 0) {
            throw std::runtime_error("Invalid read size.");
        }

        if (offset_ >= filesize_ || offset_ < 0) {
            throw std::runtime_error("Invalid offset.");
        }

        if (size_ + offset_ > filesize_) {
            throw std::runtime_error("Data out of bounds.");
        }

        // Seek to the offset
        fseek(_InternalHandle->InternalHandle, offset_, SEEK_SET);

        // Read to c string
        auto buffer = new char[size_ + 1];
        int r = fread(buffer, 1, size_, _InternalHandle->InternalHandle);

        // Null-terminate buffer
        buffer[r] = '\0';

        // Convert to string
        auto str = std::string(buffer);

        // Delete buffer
        delete[] buffer;

        return str;
    }

    bool File::WriteBytes(unsigned char *data_, int size_) {
        // Check we're open
        if (!IsOpen()) throw std::runtime_error("Cannot write to a closed file.");

        // Check for our mode
        if (_InternalOpenMode != MODE_WRITE
            && _InternalOpenMode != MODE_APPEND
            && _InternalOpenMode != MODE_READ_WRITE
            && _InternalOpenMode != MODE_READ_APPEND)
            throw std::runtime_error("File not opened for writing.");

        // Write
        return fwrite(data_, 1, size_, _InternalHandle->InternalHandle) == 1;
    }

    bool File::WriteString(const std::string &string_) {
        // Check we're open
        if (!IsOpen()) throw std::runtime_error("Cannot write to closed file.");

        // Check for our mode
        if (_InternalOpenMode != MODE_WRITE
            && _InternalOpenMode != MODE_APPEND
            && _InternalOpenMode != MODE_READ_WRITE
            && _InternalOpenMode != MODE_READ_APPEND)
            throw std::runtime_error("File not opened for writing.");

        // Write string
        return fputs(string_.c_str(), _InternalHandle->InternalHandle) != EOF;
    }

    ////////
    // Directory
    ////////

    Directory::Directory() : FilesystemObject(Path()) {}

    Directory::Directory(const Path &path_) : FilesystemObject(path_) {
        // Check for valid path
        if (!path_.Valid()) throw std::runtime_error("Directory must be given a valid path.");
    }

    bool Directory::Create() {
        auto success = false;
#if defined(_WIN32)
        // Create directory
        success = CreateDirectoryA(GetObjectPath().GetString().c_str(), nullptr) != 0;
#elif defined(__linux__) || defined(__APPLE__)
        // Create directory
        success = mkdir(GetObjectPath().GetString().c_str(), 0777) == 0;
#endif
        return success;
    }

    std::pair<bool, Directory> Directory::Create(const Path &path_) {
        auto dir = Directory(path_);
        return {dir.Create(), dir};
    }

    bool Directory::Delete() {
        // Check
        __ThrowAccessErrors();

#if defined(_WIN32)
        // Try to delete (not recursive)
        auto del = RemoveDirectoryA(ObjectPath.GetString().c_str());
        return del != 0;
#elif defined(__linux__) || defined(__APPLE__)
        return remove(ObjectPath.GetString().c_str()) == 0;
#endif
        return false;
    }

    bool Directory::DeleteRecursive() {
        // Check
        __ThrowAccessErrors();

        // Success tracker
        auto success = true;

        // Delete my own files
        for (auto file : GetFiles()) {
            if (!file.Delete()) {
                success = false;
                break;
            }
        }

        // Stop if we find an issue
        if (!success)
            return false;

        // Get directories
        auto dirs = GetDirectories();

        // Delete child directories
        for (auto dir : dirs) {
            if (!dir.DeleteRecursive()) {
                success = false;
                break;
            }
        }

        // Stop if we find an issue
        if (!success)
            return false;


        // Delete self
        success = Delete();

        // Return
        return success;
    }

    bool Directory::Exists() const {
#if defined(_WIN32)
        // https://stackoverflow.com/a/6218445
        // Get attributes for directory
        DWORD dwAttrib = GetFileAttributesA(ObjectPath.GetString().c_str());

        return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
                (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#elif defined(__linux__) || defined(__APPLE__)
        // Test opening of file
        DIR *dir = opendir(ObjectPath.GetString().c_str());
        if (dir) {
            closedir(dir);
            return true;
        }
        return false;
#endif
        return false;
    }

    Directory Directory::GetAppDataDirectory() {
        return Directory(Path::GetAppDataDirectory());
    }

    std::vector<Directory> Directory::GetDirectories() const {
        // Check
        __ThrowAccessErrors();

        // Directories vector
        auto dirs = std::vector<Directory>();
#if defined(_WIN32)
        // Find first directory
        WIN32_FIND_DATAA FindFileData;
        HANDLE hFind = FindFirstFileA((ObjectPath.GetString() + "\\*").c_str(), &FindFileData);

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
                    dirs.push_back(Directory(Path(ObjectPath, dirName)));
            }
        } while (FindNextFileA(hFind, &FindFileData) != 0);

        // Close search
        FindClose(hFind);
#elif defined(__linux__) || defined(__APPLE__)
        // Variables
        DIR *dir;
        dirent *entry;

        // Open dir
        dir = opendir(ObjectPath.GetString().c_str());

        // Test open
        if (!dir) throw std::runtime_error("Cannot open directory.");

        // Read all directories
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                    dirs.push_back(Directory(Path(ObjectPath, entry->d_name)));
            }
        }

        // Close directory
        closedir(dir);
#endif
        return dirs;
    }

    std::vector<File> Directory::GetFiles() const {
        // Check
        __ThrowAccessErrors();

        // Files vector
        auto files = std::vector<File>();
#if defined(_WIN32)
        // Find the first file in the directory
        WIN32_FIND_DATAA FindFileData;
        HANDLE hFind = FindFirstFileA((ObjectPath.GetString() + "\\*").c_str(), &FindFileData);

        // Check it exists
        if (hFind == INVALID_HANDLE_VALUE) {
            auto err = GetLastError();
            throw std::runtime_error("Invalid directory.");
        }

        // Get all files
        do {
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                auto filename = FindFileData.cFileName;
                files.push_back(File(Path(ObjectPath, filename)));
            }
        } while (FindNextFileA(hFind, &FindFileData) != 0);

        // Close search
        FindClose(hFind);
#elif defined(__linux__) || defined(__APPLE__)
        // Variables
        DIR *dir;
        dirent *entry;

        // Open dir
        dir = opendir(ObjectPath.GetString().c_str());

        // Test open
        if (!dir) throw std::runtime_error("Cannot open directory.");

        // Read all directories
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type != DT_DIR) {
                files.push_back(File(Path(ObjectPath, entry->d_name)));
            }
        }

        // Close directory
        closedir(dir);
#endif
        return files;
    }

    std::vector<File> Directory::GetFilesRecursive() const {
        // Check
        __ThrowAccessErrors();

        // Keep track of all files
        auto files = std::vector<File>();

        // Grab my files
        auto myFiles = GetFiles();
        files.insert(files.end(), myFiles.begin(), myFiles.end());

        // Get all descendant directories
        auto dirs = GetDirectories();

        for (auto dir : dirs) {
            auto dirFiles = dir.GetFilesRecursive();
            files.insert(files.end(), dirFiles.begin(), dirFiles.end());
        }

        return files;
    }

    Directory Directory::GetDirectory(const Path &path_) {
        return Directory(path_);
    }

    Directory Directory::GetExecutableDirectory() {
        return Directory(Path::GetExecutableDirectory());
    }

    Directory Directory::GetWorkingDirectory() {
        return Directory(Path::GetWorkingDirectory());
    }

    // Private Fields

    void Directory::__ThrowAccessErrors() const {
        // Check exists
        if (!Exists()) throw std::runtime_error("This directory does not exist.");

        // Check this is actually a directory
        if (GetObjectPath().GetResourceType() != TYPE_DIRECTORY) throw std::runtime_error("This path does not point to a directory.");
    }
}