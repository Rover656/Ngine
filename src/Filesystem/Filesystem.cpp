#include "Filesystem.h"

#include <stdio.h>

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
        _InternalPath = path_ + __GetJoinString() + pathB_;

        // Check path
        __CorrectPath();
    }

    TPath::TPath(const TPath& path_, const TPath& pathB_) {
        // Set internal path
        _InternalPath = path_._InternalPath + __GetJoinString() + path_._InternalPath;

        // Check path
        __CorrectPath();
    }

    // Public Methods

    std::string TPath::GetString() {
        return _InternalPath;
    }

    TPath TPath::Join(const std::string &pathA_, const std::string &pathB_) {
        return TPath(pathA_, pathB_);
    }

    TPath TPath::Join(const TPath &pathA_, const TPath &pathB_) {
        return TPath(pathA_, pathB_);
    }

    bool TPath::Valid() {
        return _HasProperValue;
    }

    // Operators

    TPath operator/(const std::string &path_, const std::string &pathB_) {
        return TPath(path_, pathB_);
    }

    TPath operator/(const TPath &path_, const TPath &pathB_) {
        return TPath(path_, pathB_);
    }

    void TPath::operator/=(const TPath &pathB_) {
        *this = TPath(*this, pathB_);
    }

    void TPath::operator/=(const std::string &pathB_) {
        *this = TPath(*this, pathB_);
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

        // TODO: Fix extra path joins
    }

    std::string TPath::__GetJoinString() {
        // TODO: See if there are any more variations
#if defined(_WIN32)
        return "\\";
#else
        return "/";
#endif
    }

    ////////
    // TFile
    ////////

    std::vector<unsigned char> TFile::ReadBytes(int size_) {
        // TODO: Test

        // Open binary file for read
        FILE *file = fopen(ObjectPath.GetString().c_str(), "rb");

        // Check for success
        if (file == nullptr) throw std::runtime_error("Unable to open file for reading.");

        // Determine file size if size is -1
        if (size_ == -1) {
            fseek(file, 0, SEEK_END);
            size_ = ftell(file);
            fseek(file, 0L, SEEK_SET);
        }

        if (size_ <= 0) {
            throw std::runtime_error("Invalid read size.");
        }

        // Read bytes to array
        unsigned char buffer[size_];
        fread(buffer, sizeof(buffer), 1, file);

        // Convert to vector
        std::vector<unsigned char> bytes(buffer, buffer + (sizeof(buffer) / sizeof(buffer[0])));

        // Close file
        fclose(file);

        // Return data
        return bytes;
    }
}