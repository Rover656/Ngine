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

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "../Config.hpp"

#include <cstdio>

namespace ngine::filesystem {
    /**
     * Resource type.
     */
    enum class ResourceType { Invalid = 0, File, Directory };

    /**
     * A path that points to a filesystem resource.
     */
    struct NEAPI Path {
        /**
         * Create a path that points nowhere.
         */
        Path();

        /**
         * Create a reference to a path.
         *
         * @param pathString The path in string form.
         */
        Path(const std::string &pathString);

        /**
         * Combine a two path strings.
         *
         * @param path Path part A.
         * @param path Path part B.
         */
        Path(const std::string &path, const std::string &pathB);

        /**
         * Combine a path together.
         *
         * @param path Path part A.
         * @param pathb_ Path part B.
         */
        Path(const Path &path, const Path &pathB);

        /**
         * Combine a path together.
         *
         * @param path Path part A.
         * @param pathB Path part B.
         */
        Path(const Path &path, const std::string &pathB);

        /**
         * Combine a path together.
         *
         * @param path Path part A.
         * @param pathB Path part B.
         */
        Path(const std::string &path, const Path &pathB);

        /**
         * Get this as an absolute path.
         * This makes the path relative to the executable directory NOT the
         * current/working directory.
         *
         * @return The absolute path from the executable directory.
         */
        Path getAbsolute() const;

        /**
         * Get app data directory.
         * This will return a varying degree of paths (Documents, Roaming app
         * data, etc.).
         *
         * @note You MUST create a folder within this path for your app data.
         * Otherwise you risk overriding other data. Read the following points
         * for details.
         * @note Desktop: Will return the \%appdata\% (or OS equivalent) path.
         * @note UWP: Will return the correct path, with no worry of overwriting
         * data.
         * @return The app data directory,
         */
        static Path getAppDataDirectory();

        /**
         * Get the executable directory.
         *
         * @return The executable directory.
         */
        static Path getExecutableDirectory();

        /**
         * Get the executable path.
         *
         * @throws std::runtime_error when UWP. Cannot get a path to the EXE.
         * @return The path to the executable file.
         */
        static Path getExecutablePath();

        /**
         * Get the file extension if present.
         *
         * @return File extension.
         */
        std::string getFileExtension() const;

        /**
         * Get referenced object name.
         *
         * @return The object name.
         */
        std::string getObjectName() const;

        /**
         * Gets a parent (if any).
         *
         * @return The parent path (if any).
         */
        Path getParent() const;

        /**
         * Get this path relative to a base.
         *
         * @param base The base path.
         * @return This path relative to the base.
         */
        Path getRelativeTo(const Path &base) const;

        /**
         * Get the type of resource this points to.
         *
         * @return The resource type.
         */
        ResourceType getResourceType() const;

        /**
         * Get string version of path.
         *
         * @return The path in string form.
         */
        std::string getString() const;

        /**
         *  Get string version of path without an extension.
         *
         *  @return The path in string form with the extension omitted (if any).
         */
        std::string getStringNoExtension() const;

        /**
         * Get the working directory.
         *
         * @return The working directory (path the game was launched from).
         */
        static Path getWorkingDirectory();

        /**
         * Whether or not the path is absolute.
         *
         * @return Whether the path is absolute or not.
         */
        bool isAbsolute() const;

        /**
         * Combine two parts of a path.
         *
         * @param pathA Path part A.
         * @param pathB Path part B.
         * @return The joined path.
         */
        static Path join(const std::string &pathA, const std::string &pathB);

        /**
         * Combine two parts of a path.
         *
         * @param pathA Path part A.
         * @param pathB Path part B.
         * @return The joined path.
         */
        static Path join(const Path &pathA, const Path &pathB);

        /**
         * Whether the path is valid or not.
         *
         * @return Whether or not the path is valid.
         */
        bool isValid() const;

        /*
         * Combine two paths
         */
        friend NEAPI Path operator/(const std::string &path,
                                    const std::string &pathB);

        /*
         * Combine two paths
         */
        friend NEAPI Path operator/(const Path &path, const Path &pathB);

        /*
         * Combine two paths
         */
        friend NEAPI Path operator/(const std::string &path, const Path &pathB);

        /*
         * Combine two paths
         */
        friend NEAPI Path operator/(const Path &path, const std::string &pathB);

        /*
         * Combine two paths
         */
        void operator/=(const Path &pathB);

        /*
         * Combine two paths
         */
        void operator/=(const std::string &pathB);

        /*
         * Implicit conversion to string
         */
        operator std::string() const;

        bool operator==(Path rhs);
        bool operator!=(Path rhs);

    private:
        /**
         * Whether or not the path points somewhere
         */
        bool m_hasProperValue = false;

        /**
         * The path string
         */
        std::string m_internalPath = "";

        /**
         * Clean a path on windows.
         * Converts Unicode name to usable name.
         *
         * @param str The path to clean.
         */
        std::string _cleanPathString(const std::string &str);

        /**
         * Run correction checks on path strings.
         */
        void _correctPath();

        /**
         * Get home directory (mac and linux)
         *
         * @return The home directory.
         */
        static std::string _getHome();

        /**
         * Get the string to join by.
         *
         * @return \\ on Windows, / on anything else.
         */
        static char _getJoinChar();
    };

    /**
     * A filesystem object
     */
    class NEAPI FilesystemObject {
    public:
        /**
         * Delete this object from the filesystem.
         *
         * @return Whether or not the object was deleted.
         */
        virtual bool deleteObject() = 0;

        /**
         * Determine whether or not this object exists on the filesystem.
         *
         * @return Returns whether or not the object exists.
         */
        virtual bool exists() const = 0;

        /**
         * Move this object.
         *
         * @param newPath The new path for the object.
         */
        void move(const Path &newPath);

        /**
         * Rename this object.
         *
         * @param newName The new name for the object.
         */
        void rename(const std::string &newName);

        /**
         * Get the name of the object.
         *
         * @return Object name.
         */
        std::string getName() const;

        /**
         * Get the object path.
         *
         * @return The object path.
         */
        Path getPath() const;

    protected:
        /**
         * Create a new filesystem object reference.
         *
         * @param path The path of the object.
         */
        explicit FilesystemObject(const Path &path);

        /**
         * The path of this object.
         */
        Path m_objectPath;
    };

    /**
     * The opening mode of a file
     */
    enum class FileOpenMode {
        /**
         * The file is not open.
         */
        Closed = 0,

        /**
         * File open for reading.
         * C file access mode r.
         */
        Read = 1,

        /**
         * File open for writing.
         * Created if not exists.
         * C file access mode w.
         */
        Write = 2,

        /**
         * File open for appending data.
         * Created if not exists.
         * C file access mode a.
         */
        Append = 3,

        /**
         * File open for reading and writing.
         * C file access mode w+.
         */
        ReadWrite = 4,

        /**
         * File open for reading and appending.
         * C file access mode a+.
         */
        ReadAppend = 5,
    };

    /**
     * A reference to a file in the filesystem.
     */
    class NEAPI File : public FilesystemObject {
        /**
         * Handler that holds the handle.
         */
        struct InternalFileHandler {
            /**
             * The internal file handle
             */
            FILE *InternalHandle = nullptr;

            ~InternalFileHandler();
        };

        /**
         * Internal handler.
         */
        std::shared_ptr<InternalFileHandler> m_internalHandle = nullptr;

        /**
         * Get the current open mode.
         */
        FileOpenMode m_internalOpenMode = FileOpenMode::Closed;

    public:
        /**
         * Create an empty file reference.
         * Points to no file.
         */
        File();

        /**
         * Create a reference to a file.
         *
         * @param path The path of the file.
         */
        File(const Path &path);

        ~File();

        /**
         * Close the file.
         */
        void close();

        /**
         * Create a new file and optionally leave it open for read or write.
         *
         * @param path Path of the new file.
         * @param leaveOpen Whether to leave the file open or not.
         * @param binary Whether or not this is a binary file.
         * @return The file.
         */
        static File createNewFile(const Path &path, bool leaveOpen = false,
                                  bool binary = true);

        /**
         * Delete this object from the filesystem.
         *
         * @return Whether or not the file was deleted.
         */
        bool deleteObject() override;

        /**
         * Determine whether or not this file exists on the filesystem.
         *
         * @return Whether the file exists or not.
         */
        bool exists() const override;

        /**
         * Get the current file mode.
         *
         * @return The current open mode.
         */
        FileOpenMode getCurrentOpenMode() const;

        /**
         * Get a file from a path.
         *
         * @param path The file path.
         * @return The file from the path.
         */
        static File getFile(const Path &path);

        /**
         * Get the file extension.
         *
         * @return The file extension.
         */
        std::string getFileExtension() const;

        /**
         * Get the raw handle to the file.
         *
         * @return C FILE handle.
         */
        FILE *getFileHandle() const;

        /**
         * Get the size of the file in bytes.
         *
         * @return The size of the file in bytes.
         */
        int getSize() const;

        /**
         * Whether or not the file is open.
         *
         * @return Whether the file is open or not.
         */
        bool isOpen() const;

        /**
         * Open the file in read or write mode.
         * Binary mode is for non-text files.
         *
         * @param mode Open mode.
         * @param binary Whether or not to open as a binary file.
         * @return Whetehr the file was opened or not.
         */
        bool open(FileOpenMode mode, bool binary = false);

        /**
         * Read a number of bytes from the file.
         * Size of -1 means all.
         *
         * @param size The size of the data to read.
         * @param offset Offset the read operation
         * @return The data from the file.
         */
        unsigned char *readBytes(int size = -1, int offset = 0);

        /**
         * Read a string from the file.
         * Size of -1 means all.
         *
         * @param size The size of the data to read.
         * @param offset Offset the read operation
         * @return The data from the file.
         */
        std::string readString(int size = -1, int offset = 0);

        /**
         * Write bytes to the file.
         *
         * @param The data to write.
         * @param The size of the data.
         * @return Whether the data was written or not.
         */
        bool writeBytes(unsigned char *data, int size);

        /**
         * Write a string to the file.
         *
         * @param The string to write.
         * @return Whether the string was written or not.
         */
        bool writeString(const std::string &string);
    };

    /**
     * A reference to a directory in the filesystem
     */
    class NEAPI Directory : public FilesystemObject {
    public:
        /**
         * Create an empty reference to a directory.
         */
        Directory();

        /**
         * Create a reference to a directory.
         *
         * @param path The path to the directory.
         */
        Directory(const Path &path);

        /**
         * Create this directory.
         *
         * @return Whether or not the directory was created.
         */
        bool create();

        /**
         * Create a new directory.
         *
         * @param path The path to the directory to create.
         * @return A pair, first is the success, second is the Directory
         * reference.
         */
        static std::pair<bool, Directory> create(const Path &path);

        /**
         * Delete this object from the filesystem.
         *
         * @note Directory must be empty to use this method. Use
         * DeleteRecursive() instead.
         * @throws std::runtime_error When directory does not exist.
         * @throws std::runtime_error When this is pointing to a non-directory
         * resource.
         * @return Whether or not the directory was deleted.
         */
        bool deleteObject() override;

        /**
         * Recursively delete the directory.
         *
         * @throws std::runtime_error When directory does not exist.
         * @throws std::runtime_error When this is pointing to a non-directory
         * resource.
         * @return Whether or not the directory and its contents were deleted.
         */
        bool deleteRecursive();

        /**
         * Determine whether or not this directory exists on the filesystem.
         *
         * @return Whether or not this directory exists.
         */
        bool exists() const override;

        /**
         * Get the app data directory for the operating system.
         * This will return a varying degree of paths (Documents, Roaming app
         * data, etc.).
         *
         * @return The AppData directory.
         * @sa ngine::filesystem::Path::GetAppDataDirectory() for platform
         * specific notes.
         */
        static Directory getAppDataDirectory();

        /**
         * Get all of the children directories.
         *
         * @throws std::runtime_error When directory does not exist.
         * @throws std::runtime_error When this is pointing to a non-directory
         * resource.
         * @return All child directories.
         */
        std::vector<Directory> getDirectories() const;

        /**
         * Get all of the children files.
         *
         * @throws std::runtime_error When directory does not exist.
         * @throws std::runtime_error When this is pointing to a non-directory
         * resource.
         * @return All child files.
         */
        std::vector<File> getFiles() const;

        /**
         * Get all of the descended children inside any child directory.
         *
         * @throws std::runtime_error When directory does not exist.
         * @throws std::runtime_error When this is pointing to a non-directory
         * resource.
         * @return All child files within *every* child folder.
         */
        std::vector<File> getFilesRecursive() const;

        /**
         * Get a directory.
         *
         * @param path The path to the directory to get.
         * @return The directory requested.
         */
        static Directory getDirectory(const Path &path);

        /**
         * Get the executable directory.
         *
         * @return The executable directory.
         * @sa ngine::filesystem::Path::GetExecutableDirectory().
         */
        static Directory getExecutableDirectory();

        /**
         * Get the working directory.
         *
         * @return The working directory (path the game was launched from).
         * @sa ngine::filesystem::Path::GetWorkingDirectory().
         */
        static Directory getWorkingDirectory();

    private:
        /**
         * Throw any errors before we access directory.
         */
        void _throwAccessErrors() const;
    };
} // namespace ngine::filesystem

#endif // FILESYSTEM_HPP
