/** @file File.h
 *
 * @copyright
 *  Copyright 2013,  Zedi Inc.  All rights reserved.
 *  This file contains confidential intellectual property of Zedi Inc.
 * @endcopyright
 *
 * @details
 *
 * @history
 * Created Jan 21, 2014 by daver:
 * @endhistory
 */

#ifndef FILE_H_
#define FILE_H_

#include <stdint.h>
#include <string>
#include <vector>
#include <dirent.h>
#include "Exception.h"

namespace zios {
namespace common {

class FileException: public Exception {
public:
    FileException(const char* funcName, int lineNumber, const std::string& message);
    virtual ~FileException() throw ();
};

class FileSystem {
public:
    FileSystem();
    ~FileSystem();

    static const char PATH_SEPARATOR;

    bool turf(const std::string& path) const;
    void normalizedPath(const std::string& path, std::string& returnPath) const;
    bool rename(const std::string& oldPath, const std::string& newPath) const;
    bool copy(const std::string& fromPath, const std::string& toPath) const;
    char* currentWorkingDirectory() const;
    bool createDirectory(const char* path) const;
    bool createDirectories(const char* path) const;
    bool touch(const char* path) const;
    inline const char pathSeparator() const {
        return PATH_SEPARATOR;
    }
    static std::string createTempFilename(const uint8_t* prefix, const uint8_t* suffix, const uint8_t* dirPath);
    static const char* tempDirectoryPath(const char* dirPath = NULL);
    static const char* homeDir();
};

class Directory {
public:
    Directory();
    Directory(const char* pathname);
    virtual ~Directory();

    /**
     * Makes the directory tree for this Directory
     *
     * @return true - if the directory was created, false if not.
     */
    virtual bool mkdirs() const;

    /** list
     *
     * Retrieve a list of filenames from a directory
     *
     * @param[in, out] returnList - a vector of strings that will hold the filenames
     *
     * @return bool - true if the list could be retrieved, false if failed.  errno will
     * be set if false returned.
     */
    virtual bool list(std::vector<std::string>& returnList) const;

    /**
     * Retrieve a list of filenames from a directory
     *
     * @param[in, out] returnList - a vector of strings that will hold the filenames
     * @param filter - a filter function to be used
     * @return bool - true if the list could be retrieved, false if failed.  errno will
     * be set if false returned.
     */
    virtual bool list(std::vector<std::string>& returnList, int (*filter)(const struct dirent *)) const;

    /**
     * Retrieve a list of filenames from a directory
     *
     * @param[in, out] returnList - a vector of strings that will hold the filenames
     * @param filter - a filter function to be used
     * @param compare - a compare function to be used
     * @return bool - true if the list could be retrieved, false if failed.  errno will
     * be set if false returned.
     */
    virtual bool list(std::vector<std::string>& returnList, int (*filter)(const struct dirent *),
            int (*compare)(const struct dirent**, const struct dirent**)) const;

    /** exists
     *
     * Tells you if the path exists and is a directory.
     *
     * @return bool - true if the path exists and is a directory.  False if either the
     * path does not exit or is not a directory.
     */
    virtual bool exists();

    /** absolutePath
     *
     *  Returns the absolute path name of the Directory.
     *
     *  @return const char* - a pointer to the full path name of the file.
     */
    virtual const char* absolutePath() const;

    /**
     * Deletes this directory.
     *
     * @param force - If true, the directory will be deleted even if it is not empty.  If
     * false, the directory will not be deleted if it contains files or subdirectories.
     *
     * @return true if delete, false otherwise.
     */
    virtual bool turf(bool force = false);

private:
    std::string _pathname;
    FileSystem _fileSystem;
};

class File {
public:
    File();
    File(const char* path, const char* filename);
    File(const char* filename);
    File(const std::string& filename);
    File(const File& otherFile);
    virtual ~File();

    /** exists
     *
     * Tells you if a file exists.
     *
     * @return bool - true if the file exists.  False if either the file does
     * not exist or you do not have permission to view the file.
     *
     * @throws FileException& - thrown if an error occurs.
     */
    virtual bool exists() const;

    /** size
     *
     * Returns the size of the file, in bytes
     *
     * @return int64_t - size of the file in bytes. -1 if an error occurs. errno contains resson
     *
     * @throws FileException& - thrown if an error occurs.
     */
    virtual int64_t size() const;

    /** name
     *
     *  Returns the name of the file.
     *
     *  @return const char* - a pointer to the name of the file.
     */
    virtual const char* name() const;

    /**
     * Performs the equivalent of a unix touch command. i.e. simply
     * creates the file.
     *
     * @return 0 on success, -1 otherwise. errno contains the error
     */
    virtual bool touch() const;

    /** absolutePath
     *
     *  Returns the absolute path name of the file.
     *
     *  @return const char* - a pointer to the full path name of the file.
     */
    virtual const char* absolutePath() const;

    virtual const std::string parent() const;

    virtual File parentFile() const;

    virtual bool renameTo(const File& file);

    virtual bool copyTo(const File& toFile);

    virtual bool mkdirs() const;

    /**
     * Returns the last modification date for a file
     *
     * @return time_t last modification date (seconds since EPOC)
     */
    virtual time_t modificationDate() const;

    /** turf
     *
     * Deletes a file from disk
     *
     * @return bool - true if successful, false if not.  errno will hold the
     * reason for hte failure.
     */
    virtual bool turf() const;

    /** isDirectory
     *
     * Tells you if the file is a direcrtory
     *
     */
    virtual bool isDirectory() const;

    /**
     * tells you if someone (u/g/o) cand run the file
     *
     */
    virtual bool isExecutable() const;

    /** list
     *
     * Lists the contents of a directory.
     *
     * @return bool - true if the file list was obtained.  false if an error is encountered.
     * errno will be set.
     *
     */
    virtual bool list(std::vector<std::string>& returnList) const;

    virtual bool list(std::vector<std::string>& returnList, int (*filter)(const struct dirent *)) const;

    virtual bool list(std::vector<std::string>& returnList, int (*filter)(const struct dirent *),
            int (*compare)(const struct dirent**, const struct dirent**)) const;

    virtual void deleteOnExit() const;

    File& operator=(const File& otherFile);

    bool operator==(const File& otherFile) const {
        return this == &otherFile;
    }

    static bool remove(const std::string& path);
    static bool exists(const char* path);
    static time_t modificationDate(const char* path);
    static std::string createTempFilename(const char* prefix, const char* suffix);
    static std::string createTempFilename(const char* prefix, const char* suffix, File* dir);
    static File createTempFile(const char* prefix, const char* suffix);
    static File createTempFile(const char* prefix, const char* suffix, File* dir);

private:
    std::string _path;
    uint32_t _prefixLength;
    FileSystem _fileSystem;
};

}
}

#endif /* FILE_H_ */
