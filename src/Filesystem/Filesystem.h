#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../ngine.h"

namespace NerdThings::Ngine::Filesystem {
    /*
     * A path that points to a filesystem resource
     */
    struct NEAPI TPath {
        // Public Constructors

        /*
         * Create a path that points nowhere
         */
        TPath();

        /*
         * Create a reference to a path
         */
        TPath(const std::string &pathString_);

        /*
         * Combine a path together
         */
        TPath(const std::string &path_, const std::string &pathB_);

        /*
         * Combine a path together
         */
        TPath(const TPath &path_, const TPath &pathB_);

        // Public Methods

        /*
         * Get string version of path
         */
        std::string GetString();

        /*
         * Combine two parts of a path
         */
        static TPath Join(const std::string &pathA_, const std::string &pathB_);

        /*
         * Combine two parts of a path
         */
        static TPath Join(const TPath &pathA_, const TPath &pathB_);

        /*
         * Whether the path is valid.
         */
        bool Valid();

        // Operators

        /*
         * Combine to paths
         */
        friend TPath operator/(const std::string &path_, const std::string &pathB_);

        /*
         * Combine to paths
         */
        friend TPath operator/(const TPath &path_, const TPath &pathB_);

        /*
         * Combine two paths
         */
        void operator/=(const TPath &pathB_);

        /*
         * Combine two paths
         */
        void operator/=(const std::string &pathB_);

    private:
        // Private Fields

        /*
         * Whether or not the path points somewhere
         */
        bool _HasProperValue = false;

        /*
         * The path string
         */
        std::string _InternalPath = "";

        // Private Methods

        /*
         * Run correction checks on path strings.
         */
        void __CorrectPath();

        /*
         * Get the string to join by
         */
        static std::string __GetJoinString();
    };

    class NEAPI TFilesystemObject {
    public:
        // Public Fields

        /*
         * The object's full name.
         * Excludes any file extensions
         */
        std::string ObjectName;

        /*
         * The path of this object
         */
        TPath ObjectPath;
    };

    class NEAPI TFile : public TFilesystemObject {
        // Private Fields

        /*
         * Whether or not the file can be read from yet.
         */
        bool _OpenForRead = false;
    public:
        // Public Fields

        /*
         * The file extension
         */
        std::string ObjectExtension;

        // Public Methods

        /*
         * Read a number of bytes from the file.
         * Size of -1 means all.
         */
        std::vector<unsigned char> ReadBytes(int size_ = -1);
    };

    class NEAPI TDirectory : public TFilesystemObject {
    public:
        // Public Methods

        /*
         * Get the contents of this directory
         */
        static std::vector<TFilesystemObject> GetContents();
    };

    /*
     * Ngine filesystem wrapper
     */
    class NEAPI Filesystem {
    public:
        // Public Methods

        /*
         * Create a new file
         */
        static TFile CreateFile(TPath path_);

        /*
         * Open a file
         */
        static TFile OpenFile(TPath path_);

        /*
         * Open or create a new file
         */
        static TFile OpenOrCreateFile(TPath path_);
    };
}

#endif //FILESYSTEM_H
