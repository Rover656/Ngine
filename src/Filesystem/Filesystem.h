#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../ngine.h"

#include <cstdio>

namespace NerdThings::Ngine::Filesystem {
    enum EResourceType {
        TYPE_INVALID = 0,
        TYPE_FILE = 1,
        TYPE_DIRECTORY = 2
    };

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

        /*
         * Combine a path together
         */
        TPath(const TPath &path_, const std::string &pathB_);

        /*
         * Combine a path together
         */
        TPath(const std::string &path_, const TPath &pathB_);

        // Public Methods

        /*
         * Get the executable directory
         */
        static TPath GetExecutableDirectory();

        /*
         * Get the executable path
         */
        static TPath GetExecutablePath();

        /*
         * Get the file extension if present
         */
        std::string GetFileExtension() const;

        /*
         * Get referenced object name
         */
        std::string GetObjectName() const;

        /*
         * Gets a parent (if any)
         */
        TPath GetParent() const;

        /*
         * Get this path relative to a base.
         */
        TPath GetRelativeTo(const TPath &base_) const;

        /*
         * Get the type of resource this points to
         */
        EResourceType GetResourceType() const;

        /*
         * Get string version of path
         */
        std::string GetString() const;

        /*
         *  Get string version of path without an extenson
         */
        std::string GetStringNoExtension() const;

        /*
         * Get the working directory.
         */
        static TPath GetWorkingDirectory();

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
        bool Valid() const;

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
         * Combine to paths
         */
        friend TPath operator/(const std::string &path_, const TPath &pathB_);

        /*
         * Combine to paths
         */
        friend TPath operator/(const TPath &path_, const std::string &pathB_);

        /*
         * Combine two paths
         */
        void operator/=(const TPath &pathB_);

        /*
         * Combine two paths
         */
        void operator/=(const std::string &pathB_);

        /*
         * Implicit conversion to string
         */
        operator std::string() const;

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
         * Clean a path on windows.
         * Converts Unicode name to usable name.
         */
        std::string __CleanPathString(const std::string &str_);

        /*
         * Run correction checks on path strings.
         */
        void __CorrectPath();

        /*
         * Get the string to join by
         */
        static char __GetJoinChar();
    };

    /*
     * A filesystem object
     */
    class NEAPI TFilesystemObject {
    public:
        // Public Methods

        /*
         * Delete this object from the filesystem.
         */
        virtual bool Delete() = 0;

        /*
         * Determine whether or not this object exists on the filesystem.
         */
        virtual bool Exists() = 0;

        /*
         * Move this object.
         */
        void Move(const TPath &newPath_);

        /*
         * Rename this object.
         */
        void Rename(const std::string &newName_);

        /*
         * Get the name of the object
         */
        std::string GetObjectName();

        /*
         * Get the object path
         */
        TPath GetObjectPath();
    protected:
        // Protected Constructors

        /*
         * Create a new filesystem object reference.
         */
        explicit TFilesystemObject(const TPath &path_);

        // Protected Fields

        /*
         * The path of this object
         */
        TPath ObjectPath;
    };

    /*
     * The opening mode of a file
     */
    enum EFileOpenMode {
        /*
         * The file is not open.
         */
        MODE_NONE = 0,

        /*
         * File open for reading.
         * C file access mode r.
         */
        MODE_READ = 1,

        /*
         * File open for writing.
         * Created if not exists.
         * C file access mode w.
         */
        MODE_WRITE = 2,

        /*
         * File open for appending data.
         * Created if not exists.
         * C file access mode a.
         */
        MODE_APPEND = 3,

        /*
         * File open for reading and writing.
         * C file access mode w+.
         */
        MODE_READ_WRITE = 4,

        /*
         * File open for reading and appending.
         * C file access mode a+.
         */
        MODE_READ_APPEND = 5,
    };

    /*
     * A reference to a file in the filesystem
     */
    class NEAPI TFile : public TFilesystemObject {
        // Private Structs

        /*
         * Handler that holds the handle.
         */
        struct InternalFileHandler {
            /*
             * The internal file handle
             */
            FILE *InternalHandle = nullptr;

            // Destructor

            ~InternalFileHandler();
        };

        // Private Fields

        /*
         * Internal handler
         */
        std::shared_ptr<InternalFileHandler> _InternalHandle = nullptr;

        /*
         * Get the current open mode
         */
        EFileOpenMode  _InternalOpenMode = MODE_NONE;
    public:
        // Public Constructors

        /*
         * Create an empty file reference.
         * Points to no file.
         */
        TFile();

        /*
         * Create a reference to a file.
         */
        TFile(const TPath &path_);

        // Destructor

        ~TFile();

        // Public Methods

        /*
         * Close the file
         */
        void Close();

        /*
         * Create a new file and optionally leave it open for read or write.
         */
        static TFile CreateNewFile(TPath path_, bool leaveOpen_ = false);

        /*
         * Delete this object from the filesystem.
         */
        bool Delete() override;

        /*
         * Determine whether or not this object exists on the filesystem.
         */
        bool Exists() override;

        /*
         * Get the current file mode
         */
        EFileOpenMode GetCurrentMode();

        /*
         * Get a file from a path
         */
        static TFile GetFile(const TPath &path_);

        /*
         * Get the file extension
         */
        std::string GetFileExtension();

        /*
         * Get the raw handle to the file.
         */
        FILE *GetFileHandle();

        /*
         * Get the size of the file
         */
        int GetSize();

        /*
         * Whether or not the file is open
         */
        bool IsOpen();

        /*
         * Open the file in read or write mode.
         * Binary mode is for non-text files.
         */
        bool Open(EFileOpenMode mode_, bool binary_ = false);

        /*
         * Read a number of bytes from the file.
         * Size of -1 means all.
         */
        std::vector<unsigned char> ReadBytes(int size_ = -1, int offset_ = 0);

        /*
         * Read a string from the file.
         * Size of -1 means all.
         */
        std::string ReadString(int size_ = -1, int offset_ = 0);

        /*
         * Write bytes to the file.
         */
        bool WriteBytes(std::vector<unsigned char> data_);

        /*
         * Write a string to the file.
         */
        bool WriteString(const std::string &string_);
    };

    /*
     * A reference to a directory in the filesystem
     */
    class NEAPI TDirectory : public TFilesystemObject {
    public:
        // Public Constructors

        /*
         * Create a null reference to a directory
         */
        TDirectory();

        /*
         * Create a reference to a directory
         */
        TDirectory(const TPath &path_);

        // Public Methods

        /*
         * Create a new directory.
         * Returns success and the new directory (may be reference to nothing).
         */
        static std::pair<bool, TDirectory> Create(const TPath &path_);

        /*
         * Delete this object from the filesystem.
         * Fails if directory is not empty
         */
        bool Delete() override;

        /*
         * Recursivly delete the folder.
         */
        bool DeleteRecursive();

        /*
         * Determine whether or not this object exists on the filesystem.
         */
        bool Exists() override;

        /*
         * Get all of the children directories.
         */
        std::vector<TDirectory> GetDirectories();

        /*
         * Get all of the children files.
         */
        std::vector<TFile> GetFiles();

        /*
         * Get all of the descended children inside any child directory.
         */
        std::vector<TFile> GetFilesRecursive();

        /*
         * Get a directory
         */
        static TDirectory GetDirectory(const TPath &path_);

        /*
         * Get the executable directory.
         */
        static TDirectory GetExecutableDirectory();

        /*
         * Get the working directory for the program.
         */
        static TDirectory GetWorkingDirectory();
    };
}

#endif //FILESYSTEM_H
