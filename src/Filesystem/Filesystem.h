#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "../ngine.h"

#include <cstdio>

namespace NerdThings::Ngine::Filesystem {
    enum ResourceType {
        TYPE_INVALID = 0,
        TYPE_FILE = 1,
        TYPE_DIRECTORY = 2
    };

    /*
     * A path that points to a filesystem resource
     */
    struct NEAPI Path {
        // Public Constructors

        /*
         * Create a path that points nowhere
         */
        Path();

        /*
         * Create a reference to a path
         */
        Path(const std::string &pathString_);

        /*
         * Combine a path together
         */
        Path(const std::string &path_, const std::string &pathB_);

        /*
         * Combine a path together
         */
        Path(const Path &path_, const Path &pathB_);

        /*
         * Combine a path together
         */
        Path(const Path &path_, const std::string &pathB_);

        /*
         * Combine a path together
         */
        Path(const std::string &path_, const Path &pathB_);

        // Public Methods

        /*
         * Get this as an absolute path.
         * This makes the path relative to the executable directory NOT the current/working directory.
         */
        Path GetAbsolute() const;

        /*
         * Get app data directory.
         * This will return a varying degree of paths (Documents, Roaming app data, etc.).
         * NOTE: You MUST create a folder within this path for your app data. Otherwise you risk overriding other data.
         */
        static Path GetAppDataDirectory();

        /*
         * Get the executable directory
         */
        static Path GetExecutableDirectory();

        /*
         * Get the executable path
         */
        static Path GetExecutablePath();

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
        Path GetParent() const;

        /*
         * Get this path relative to a base.
         */
        Path GetRelativeTo(const Path &base_) const;

        /*
         * Get the type of resource this points to
         */
        ResourceType GetResourceType() const;

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
        static Path GetWorkingDirectory();

        /*
         * Whether or not the path is absolute.
         */
        bool IsAbsolute() const;

        /*
         * Combine two parts of a path
         */
        static Path Join(const std::string &pathA_, const std::string &pathB_);

        /*
         * Combine two parts of a path
         */
        static Path Join(const Path &pathA_, const Path &pathB_);

        /*
         * Whether the path is valid.
         */
        bool Valid() const;

        // Operators

        /*
         * Combine to paths
         */
        friend Path operator/(const std::string &path_, const std::string &pathB_);

        /*
         * Combine to paths
         */
        friend Path operator/(const Path &path_, const Path &pathB_);

        /*
         * Combine to paths
         */
        friend Path operator/(const std::string &path_, const Path &pathB_);

        /*
         * Combine to paths
         */
        friend Path operator/(const Path &path_, const std::string &pathB_);

        /*
         * Combine two paths
         */
        void operator/=(const Path &pathB_);

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
         * Get home directory (mac and linux)
         */
        static std::string __GetHome();

        /*
         * Get the string to join by
         */
        static char __GetJoinChar();
    };

    /*
     * A filesystem object
     */
    class NEAPI FilesystemObject {
    public:
        // Public Methods

        /*
         * Delete this object from the filesystem.
         */
        virtual bool Delete() = 0;

        /*
         * Determine whether or not this object exists on the filesystem.
         */
        virtual bool Exists() const = 0;

        /*
         * Move this object.
         */
        void Move(const Path &newPath_);

        /*
         * Rename this object.
         */
        void Rename(const std::string &newName_);

        /*
         * Get the name of the object
         */
        std::string GetObjectName() const;

        /*
         * Get the object path
         */
        Path GetObjectPath() const;
    protected:
        // Protected Constructors

        /*
         * Create a new filesystem object reference.
         */
        explicit FilesystemObject(const Path &path_);

        // Protected Fields

        /*
         * The path of this object
         */
        Path ObjectPath;
    };

    /*
     * The opening mode of a file
     */
    enum FileOpenMode {
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
    class NEAPI File : public FilesystemObject {
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
        FileOpenMode  _InternalOpenMode = MODE_NONE;
    public:
        // Public Constructors

        /*
         * Create an empty file reference.
         * Points to no file.
         */
        File();

        /*
         * Create a reference to a file.
         */
        File(const Path &path_);

        // Destructor

        ~File();

        // Public Methods

        /*
         * Close the file
         */
        void Close();

        /*
         * Create a new file and optionally leave it open for read or write.
         */
        static File CreateNewFile(const Path &path_, bool leaveOpen_ = false, bool binary_ = true);

        /*
         * Delete this object from the filesystem.
         */
        bool Delete() override;

        /*
         * Determine whether or not this object exists on the filesystem.
         */
        bool Exists() const override;

        /*
         * Get the current file mode
         */
        FileOpenMode GetCurrentMode() const;

        /*
         * Get a file from a path
         */
        static File GetFile(const Path &path_);

        /*
         * Get the file extension
         */
        std::string GetFileExtension() const;

        /*
         * Get the raw handle to the file.
         */
        FILE *GetFileHandle() const;

        /*
         * Get the size of the file
         */
        int GetSize() const;

        /*
         * Whether or not the file is open
         */
        bool IsOpen() const;

        /*
         * Open the file in read or write mode.
         * Binary mode is for non-text files.
         */
        bool Open(FileOpenMode mode_, bool binary_ = false);

        /*
         * Read a number of bytes from the file.
         * Size of -1 means all.
         */
        unsigned char *ReadBytes(int size_ = -1, int offset_ = 0);

        /*
         * Read a string from the file.
         * Size of -1 means all.
         */
        std::string ReadString(int size_ = -1, int offset_ = 0);

        /*
         * Write bytes to the file.
         */
        bool WriteBytes(unsigned char *data_, int size_);

        /*
         * Write a string to the file.
         */
        bool WriteString(const std::string &string_);
    };

    /*
     * A reference to a directory in the filesystem
     */
    class NEAPI Directory : public FilesystemObject {
    public:
        // Public Constructors

        /*
         * Create a null reference to a directory
         */
        Directory();

        /*
         * Create a reference to a directory
         */
        Directory(const Path &path_);

        // Public Methods

        /*
         * Create a new directory.
         * Returns success and the new directory (may be reference to nothing).
         */
        static std::pair<bool, Directory> Create(const Path &path_);

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
        bool Exists() const override;

        /*
         * Get the app data directory for the operating system.
         * This will return a varying degree of paths (Documents, Roaming app data, etc.).
         * NOTE: You MUST create a folder within this path for your app data. Otherwise you risk overriding other data.
         */
        static Directory GetAppDataDirectory();

        /*
         * Get all of the children directories.
         */
        std::vector<Directory> GetDirectories() const;

        /*
         * Get all of the children files.
         */
        std::vector<File> GetFiles() const;

        /*
         * Get all of the descended children inside any child directory.
         */
        std::vector<File> GetFilesRecursive() const;

        /*
         * Get a directory
         */
        static Directory GetDirectory(const Path &path_);

        /*
         * Get the executable directory.
         */
        static Directory GetExecutableDirectory();

        /*
         * Get the working directory for the program.
         */
        static Directory GetWorkingDirectory();
    };
}

#endif //FILESYSTEM_H
