#include "Filesystem.h"

#if defined(_WIN32)
#include <windows.h>
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

namespace NerdThings::Ngine::Filesystem {
    ////////
    // TPath
    ////////

    // Public Constructors

    TPath::TPath() {
        // Mark as improper
        _HasProperValue = false;
    }

    TPath::TPath(const std::string &pathString_) {
        // Set internal path
        _InternalPath = pathString_;

        // Check path
        __CorrectPath();
    }

    TPath::TPath(const std::string &path_, const std::string &pathB_) {
        // Set internal path
        _InternalPath = path_ + __GetJoinChar() + pathB_;

        // Check path
        __CorrectPath();
    }

    TPath::TPath(const TPath& path_, const TPath& pathB_) {
        // Set internal path
        _InternalPath = path_._InternalPath + __GetJoinChar() + pathB_._InternalPath;

        // Check path
        __CorrectPath();
    }

    TPath::TPath(const TPath &path_, const std::string &pathB_) {
        // Set internal path
        _InternalPath = path_._InternalPath + __GetJoinChar() + pathB_;

        // Check path
        __CorrectPath();
    }

    TPath::TPath(const std::string &path_, const TPath &pathB_) {
        // Set internal path
        _InternalPath = path_ + __GetJoinChar() + pathB_._InternalPath;

        // Check path
        __CorrectPath();
    }

    // Public Methods

    TPath TPath::GetExecutableDirectory() {
        return GetExecutablePath().GetParent();
    }

    TPath TPath::GetExecutablePath() {
        // https://github.com/cginternals/cpplocate/blob/master/source/liblocate/source/liblocate.c#L39
        // I trust this works as there are no issues about it...
#if defined(_WIN32) && defined(PLATFORM_DESKTOP)
        char exePath[MAX_PATH];
        unsigned int len = GetModuleFileNameA(GetModuleHandleA(nullptr), exePath, MAX_PATH);
        if (len == 0)
            return TPath("");

        return TPath(std::string(exePath));
#elif defined(__linux__)
        char exePath[PATH_MAX];
        int len = readlink("/proc/self/exe", exePath, PATH_MAX);
        if (len <= 0 || len == PATH_MAX)
            return TPath("");

        success_ = true;
        return TPath(std::string(exePath));
#elif defined(__APPLE__)
        char exePath[PATH_MAX];

        unsigned int len = (unsigned int)PATH_MAX;

        if (_NSGetExecutablePath(exePath, &len) == 0)
        {
            char * realPath = realpath(exePath, 0x0);

            if (realPath == 0x0)
            {
                return TPath("");
            }

            auto pathStr = std::string(realPath);

            free(realPath);

            success_ = true;
            return TPath(pathStr);
        }
        else
        {
            char * intermediatePath = (char *)malloc(sizeof(char) * len);

            if (_NSGetExecutablePath(intermediatePath, &len) != 0)
            {
                free(intermediatePath);
                return TPath("");
            }

            char * realPath = realpath(intermediatePath, 0x0);

            free(intermediatePath);

            if (realPath == 0x0)
            {
                return TPath("");
            }

            auto pathStr = std::string(realPath);

            free(realPath);

            success_ = true;
            return TPath(pathStr);
        }

        #else
        // Returns blank, this cannot be used
        return TPath("");
#endif
    }

    std::string TPath::GetFileExtension() const {
        // Get path string
        auto path = GetString();

        // Remove the file extension
        auto dot = path.find_last_of('.');
        if (std::string::npos != dot) {
            return path.substr(dot + 1);
        } else {
            return "";
        }
    }

    std::string TPath::GetObjectName() const {
        // Get path string
        auto nameTemp = GetString();

        // Search for the last directory slash
        auto fSlash = nameTemp.find_last_of(__GetJoinChar());
        if (std::string::npos != fSlash) {
            nameTemp.erase(0, fSlash);
        }

        // Remove the file extension
        auto dot = nameTemp.find_last_of('.');
        if (std::string::npos != dot) {
            nameTemp.erase(dot);
        }

        return nameTemp;
    }

    TPath TPath::GetParent() {
        auto lastSlash = _InternalPath.find_last_of(__GetJoinChar());

        if (std::string::npos != lastSlash)
            return TPath(_InternalPath.substr(0, lastSlash));
        else return TPath();
    }

    std::string TPath::GetString() const {
        return _InternalPath;
    }

    TPath TPath::Join(const std::string &pathA_, const std::string &pathB_) {
        return TPath(pathA_, pathB_);
    }

    TPath TPath::Join(const TPath &pathA_, const TPath &pathB_) {
        return TPath(pathA_, pathB_);
    }

    bool TPath::Valid() const {
        return _HasProperValue;
    }

    // Operators

    TPath operator/(const std::string &path_, const std::string &pathB_) {
        return TPath(path_, pathB_);
    }

    TPath operator/(const TPath &path_, const TPath &pathB_) {
        return TPath(path_, pathB_);
    }

    TPath operator/(const std::string &path_, const TPath &pathB_) {
        return TPath(path_, pathB_);
    }

    TPath operator/(const TPath &path_, const std::string &pathB_) {
        return TPath(path_, pathB_);
    }

    void TPath::operator/=(const TPath &pathB_) {
        // Set internal path
        _InternalPath = _InternalPath + __GetJoinChar() + pathB_._InternalPath;

        // Check path
        __CorrectPath();
    }

    void TPath::operator/=(const std::string &pathB_) {
        // Set internal path
        _InternalPath = _InternalPath + __GetJoinChar() + pathB_;

        // Check path
        __CorrectPath();
    }

    TPath::operator std::string() const {
        return _InternalPath;
    }

    // Private Methods

    void TPath::__CorrectPath() {
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

    char TPath::__GetJoinChar() {
        // TODO: See if there are any more variations
#if defined(_WIN32)
        return '\\';
#else
        return '/';
#endif
    }

    ////////
    // TFilesystemObject
    ////////

    // Public Methods

    std::string TFilesystemObject::GetObjectName() {
        return ObjectPath.GetObjectName();
    }

    TPath TFilesystemObject::GetObjectPath() {
        return ObjectPath;
    }

    // Protected Constructors

    TFilesystemObject::TFilesystemObject(const TPath &path_) : ObjectPath(path_) {}

    ////////
    // TFile
    ////////

    // Public Constructor(s)

    TFile::TFile() : TFilesystemObject(TPath()) {}

    TFile::TFile(const TPath &path_) : TFilesystemObject(path_) {
        // Check path is valid
        if (!path_.Valid()) throw std::runtime_error("Cannot use invalid paths for TFile.");
    }

    // Public Methods

    void TFile::Close() {
        if (_InternalHandle != nullptr)
            fclose(_InternalHandle);

        // Set mode
        _InternalOpenMode = MODE_NONE;
    }

    EFileOpenMode TFile::GetCurrentMode() {
        return _InternalOpenMode;
    }

    TFile TFile::GetFile(const TPath &path_) {
        return TFile(path_);
    }

    int TFile::GetSize() {
        if (!IsOpen()) {
            ConsoleMessage("Cannot determine size of file that is not open.", "WARN", "TFile");
            return 0;
        }

        fseek(_InternalHandle, 0, SEEK_END);
        auto s = ftell(_InternalHandle);
        fseek(_InternalHandle, 0, SEEK_SET);

        return s;
    }

    bool TFile::IsOpen() {
        return _InternalHandle != nullptr;
    }

    bool TFile::Open(EFileOpenMode mode_, bool binary_) {
        // Check validity of path
        if (!ObjectPath.Valid()) throw std::runtime_error("This file's path is invalid");

        // Open with selected mode
        switch(mode_) {
            case MODE_READ:
                // Open binary file for read
                _InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "rb" : "r");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_WRITE:
                // Open binary file for write
                _InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "wb" : "w");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_APPEND:
                // Open binary file for append
                _InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "ab" : "a");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_READ_WRITE:
                // Open binary file for append
                _InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "w+b" : "w+");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            case MODE_READ_APPEND:
                // Open binary file for append
                _InternalHandle = fopen(ObjectPath.GetString().c_str(), binary_ ? "a+b" : "a+");

                // Set mode
                _InternalOpenMode = mode_;
                break;
            default:
                ConsoleMessage("File mode not supported.", "WARN", "TFile");

                // Set mode
                _InternalOpenMode = MODE_NONE;
                break;
        }

        // Return success
        return IsOpen();
    }

    std::vector<unsigned char> TFile::ReadBytes(int size_, int offset_) {
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

        if (offset_ >= filesize_) {
            throw std::runtime_error("Invalid offset.");
        }

        // Seek to the offset
        fseek(_InternalHandle, offset_, SEEK_SET);

        // Read bytes to array
        auto *buffer = new unsigned char[size_];
        fread(buffer, 1, sizeof(buffer), _InternalHandle);

        // Convert to vector
        std::vector<unsigned char> bytes(buffer, buffer + (sizeof(buffer) / sizeof(buffer[0])));

        // Delete buffer
        delete[] buffer;

        // Return data
        return bytes;
    }

    std::string TFile::ReadString(int size_, int offset_) {
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

        if (offset_ >= filesize_) {
            throw std::runtime_error("Invalid offset.");
        }

        // Seek to the offset
        fseek(_InternalHandle, offset_, SEEK_SET);

        // Read to c string
        auto buffer = new char[size_ + 1];
        int r = fread(buffer, 1, size_, _InternalHandle);

        // Null-terminate buffer
        buffer[r] = '\0';

        // Convert to string
        auto str = std::string(buffer);

        // Delete buffer
        delete[] buffer;

        return str;
    }

    bool TFile::WriteBytes(std::vector<unsigned char> data_) {
        // Check we're open
        if (!IsOpen()) throw std::runtime_error("Cannot write to a closed file.");

        // Check for our mode
        if (_InternalOpenMode != MODE_WRITE
            && _InternalOpenMode != MODE_APPEND
            && _InternalOpenMode != MODE_READ_WRITE
            && _InternalOpenMode != MODE_READ_APPEND)
            throw std::runtime_error("File not opened for writing.");

        // Calculate size
        auto dataSize = data_.size() * sizeof(unsigned char);

        // Write
        return fwrite(data_.data(), 1, dataSize, _InternalHandle) == 1;
    }

    bool TFile::WriteString(const std::string &string_) {
        // Check we're open
        if (!IsOpen()) throw std::runtime_error("Cannot write to closed file.");

        // Check for our mode
        if (_InternalOpenMode != MODE_WRITE
            && _InternalOpenMode != MODE_APPEND
            && _InternalOpenMode != MODE_READ_WRITE
            && _InternalOpenMode != MODE_READ_APPEND)
            throw std::runtime_error("File not opened for writing.");

        // Write string
        return fputs(string_.c_str(), _InternalHandle) != EOF;
    }

    ////////
    // TDirectory
    ////////
}