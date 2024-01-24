/** @file File.cpp
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
 * BugFix_ Oct 22, 2014 by eb:    fix mkdir in File to not create subdir with file name
 * BugFix_ Nov 20, 2014 by eb:    fix mem leak in File:List
 * BugFix_ Sep 16, 2015 by eb:    change dir make to make group rwx also.
 * @endhistory
 */

#include "File.h"

#include "Mutex.h"
#include "Random.h"
#include "templates.h"

#include <asm-generic/errno-base.h>
#include <pwd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <fstream>

namespace zios {
namespace common {

using namespace std;

static Mutex mutex;
static std::vector<string>* deleteFileNames = NULL;
static const char SLASH = '/';
static const char TILDE = '~';
static const char* DEFAULT_TEMP_DIR = "/tmp/";

const char FileSystem::PATH_SEPARATOR = '/';

static bool fileExists(const char* path) {
    struct stat sb;
    return stat(path, &sb) == 0;
}

void deleteFileOnExitHandler(void) {
    for (std::vector<string>::iterator iter = deleteFileNames->begin(); iter != deleteFileNames->end(); ++iter)
        ::unlink((*iter).c_str());
    delete deleteFileNames;
}

FileException::FileException(const char* funcName, int lineNumber, const std::string& message) :
        Exception(funcName, lineNumber, message) {
}

FileException::~FileException() throw () {
}

Directory::Directory() {
    FileSystem fileSystem;
    _pathname = fileSystem.currentWorkingDirectory();
}

Directory::Directory(const char* pathname) : _pathname(pathname) {
    if (::strcmp(".", pathname) == 0) {
        FileSystem fileSystem;
        char* curDir = fileSystem.currentWorkingDirectory();
        _pathname = curDir;
        ::free(curDir);
    }
}

Directory::~Directory() {
}

bool Directory::mkdirs() const {
    if (_pathname.size() > 0) {
        std::string absolutePath;
        if (_pathname[0] != _fileSystem.pathSeparator()) {
            char* curDir = _fileSystem.currentWorkingDirectory();
            absolutePath.append(curDir).push_back(_fileSystem.pathSeparator());
            absolutePath.append(_pathname);
            ::free(curDir);
        } else
            absolutePath = _pathname;
        return _fileSystem.createDirectories(absolutePath.c_str());
    }
    return false;
}

bool Directory::exists() {
    struct stat sb;
    int32_t status = stat(_pathname.c_str(), &sb);
    if (status == -1) {
        if (errno == ENOENT)
            return false;
        else
            THROW_NAMED_EXCEPTION(FileException, ::strerror(errno));
    }
    return S_ISDIR(sb.st_mode);
}


bool Directory::list(vector<std::string>& returnList,
                int (*filter)(const struct dirent *),
                int (*compare)(const struct dirent**, const struct dirent**)) const {

    struct dirent **namelist;
    int n;

    n = ::scandir(_pathname.c_str(), &namelist, filter, compare);
    if (n < 0)
        return false;
    else {
        while (n--)
            returnList.push_back(namelist[n]->d_name);
        free(namelist);
    }
    return true;
}

bool Directory::turf(bool force) {
    string command("rm ");
    if (force)
        command.append("-rf ");
    command.append(absolutePath()).append(" 2> /dev/null");
    return ::system(command.c_str()) == 0;
}

bool Directory::list(vector<std::string>& returnList,
                int (*filter)(const struct dirent *)) const {
    return list(returnList, filter, NULL);
}

bool Directory::list(vector<std::string>& returnList) const {
    return list(returnList, NULL);
}

const char* Directory::absolutePath() const {
    return _pathname.c_str();
}

File::File() : _prefixLength(0) {
}

File::File(const char* path, const char* filename) :
        _prefixLength(0) {
    string fn(path);
    _fileSystem.normalizedPath(fn, _path);
    _path.append(&SLASH, 1).append(filename);
    _prefixLength = _path.find_last_of(SLASH);
}

File::File(const char* filename) :
        _prefixLength(0) {
    string fn(filename);
    _fileSystem.normalizedPath(fn, _path);
    _prefixLength = _path.find_last_of(SLASH);
}

File::File(const std::string& filename) :
        _prefixLength(0) {
    _fileSystem.normalizedPath(filename, _path);
    _prefixLength = _path.find_last_of(SLASH);
}

File::File(const File& otherFile) :
        _path(otherFile._path),
        _prefixLength(otherFile._prefixLength) {
}

File::~File() {
}

const char* File::name() const {
    return &_path.c_str()[_prefixLength+1];
}

bool File::touch() const {
    return _fileSystem.touch(absolutePath());
}

const char* File::absolutePath() const {
    return _path.c_str();
}

const std::string File::parent() const {
    return _path.substr(0, _prefixLength);
}

File File::parentFile() const {
    return File(parent());
}

bool File::renameTo(const File& file) {
    return _fileSystem.rename(_path.c_str(), file._path.c_str());
}

bool File::mkdirs() const {
    return _fileSystem.createDirectories(parent().c_str());
}

bool File::copyTo(const File& toFile) {
    return _fileSystem.copy(_path.c_str(), toFile._path.c_str());
}

File& File::operator=(const File& otherFile) {
    _path = otherFile._path;
    _prefixLength = otherFile._prefixLength;
    return *this;
}

void File::deleteOnExit() const {
    if (deleteFileNames == NULL) {
        ScopedLock<Mutex> lock(mutex);
        // It may look odd to check again, but another thread might
        // be doing the same thing when we do this.
        if (deleteFileNames == NULL) {
            deleteFileNames = new std::vector<string>();
            ::atexit(deleteFileOnExitHandler);
        }
    }
    deleteFileNames->push_back(_path);
}

bool File::exists() const {
    return fileExists(_path.c_str());
}

time_t File::modificationDate() const {
    return modificationDate(_path.c_str());
}

int64_t File::size() const {
    struct stat sb;
    return (stat(_path.c_str(), &sb) == -1) ? -1 : sb.st_size;
}

bool File::isDirectory() const {
    struct stat sb;
    int32_t status = stat(_path.c_str(), &sb);
    if (status == -1) {
        if (errno == ENOENT)
            return false;
        else
            THROW_NAMED_EXCEPTION(FileException, ::strerror(errno));
    }
    return S_ISDIR(sb.st_mode);
}

bool File::isExecutable() const {
    struct stat sb;
    int32_t status = stat(_path.c_str(), &sb);
    if (status == -1) {
        if (errno == ENOENT)
            return false;
        else
            THROW_NAMED_EXCEPTION(FileException, ::strerror(errno));
    }
    //todo: enhance so we only check relevant bits, if we own file or are group, or other...
    return ((sb.st_mode & (S_IXUSR|S_IXGRP|S_IXOTH)) != 0);
}


bool File::list(vector<std::string>& returnList,
                int (*filter)(const struct dirent *),
                int (*compare)(const struct dirent**, const struct dirent**)) const {

    struct dirent **namelist;
    int n;

    n = ::scandir(_path.c_str(), &namelist, filter, compare);
    if (n < 0)
        return false;
    else {
        while (n--){
            returnList.push_back(namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }
    return true;
}

bool File::list(vector<std::string>& returnList,
                int (*filter)(const struct dirent *)) const {
    return list(returnList, filter, NULL);
}

bool File::list(vector<std::string>& returnList) const {
    return list(returnList, NULL);
}

bool File::turf() const {
    return _fileSystem.turf(_path);
}

File File::createTempFile(const char* prefix, const char* suffix) {
    return createTempFile(prefix, suffix, NULL);
}

File File::createTempFile(const char* prefix, const char* suffix, File* dir) {
    assert(prefix != NULL);
    assert(suffix != NULL);
    return File(createTempFilename(prefix, suffix, dir));
}

string File::createTempFilename(const char* prefix, const char* suffix) {
    return createTempFilename(prefix, suffix, NULL);
}

string File::createTempFilename(const char* prefix, const char* suffix, File* dir) {
    assert(prefix != NULL);
    assert(suffix != NULL);
    const char* dirPath = (dir == NULL) ? NULL : dir->name();
    return FileSystem::createTempFilename((const uint8_t*)prefix, (const uint8_t*)suffix, (const uint8_t*)dirPath);
}

bool File::exists(const char* path) {
    return fileExists(path);
}

bool File::remove(const string& path)
{
    return ::remove(path.c_str()) == 0;
}

time_t File::modificationDate(const char* path){
    struct stat sb;
    stat(path, &sb);
    return sb.st_mtim.tv_sec;
}


FileSystem::FileSystem() {
}

FileSystem::~FileSystem() {
}

bool FileSystem::touch(const char* path) const {
    ofstream ofs(path);
    if (false == ofs.is_open())
        return false;
    ofs.close();
    return true;
}

bool FileSystem::createDirectories(const char* absolutePath) const {
    string path(absolutePath);
    string::size_type pos = 0;
    while ((pos = path.find_first_of(SLASH, pos+1)) != string::npos) {
        if (createDirectory(path.substr(0, pos).c_str()) == false)
            return false;
    }
    return createDirectory(absolutePath);
}

bool FileSystem::createDirectory(const char* path) const {
    if (fileExists(path))
        return true;
    mode_t mode = S_IRWXU | S_IRWXG | S_IROTH;
    // It may seem redundant to use both calls but mkdir seems to ignore any mode
    // setting other than owner.
    return ::mkdir(path, 0) == 0 && ::chmod(path, mode) == 0;
}

string FileSystem::createTempFilename(const uint8_t* prefix, const uint8_t* suffix, const uint8_t* dirPath) {
    ostringstream oss;
    Random random;
    const char* dirName = tempDirectoryPath((char*)dirPath);
    oss << dirName << prefix << random.nextU32() << suffix;
    return std::move(oss.str());
}

const char* FileSystem::tempDirectoryPath(const char* dirPath)
{
    return (dirPath == NULL) ? DEFAULT_TEMP_DIR : (const char*)dirPath;
}

const char* FileSystem::homeDir() {
    const char* homedir = NULL;
    if ((homedir = ::getenv("HOME")) == NULL)
        homedir = ::getpwuid(::getuid())->pw_dir;
    return homedir;
}

bool FileSystem::turf(const string& path) const {
    return ::remove(path.c_str()) == 0;
}

bool FileSystem::rename(const std::string& oldPath, const std::string& newPath) const {
    return ::rename(oldPath.c_str(), newPath.c_str()) == 0;
}

bool FileSystem::copy(const std::string& fromPath, const std::string& toPath) const {
    string command("cp ");
    command.append(fromPath).append(" ");
    command.append(toPath);
    errno = 1;
    return ::system(command.c_str()) == 0;
}


char* FileSystem::currentWorkingDirectory() const {
    return ::get_current_dir_name();
}

void FileSystem::normalizedPath(const string& path, string& returnPath) const {
    if (path.size() > 0 && path.at(0) == SLASH) {
        returnPath.append(path);
    } else if (path.size() > 0 && path.at(0) == TILDE) {
        returnPath = path;
        returnPath.replace(0, 1, homeDir());
    }else {
        char* cwd = ::get_current_dir_name();
        returnPath.append(cwd);
        if (path.size() > 0 )
            returnPath.append((char*)&SLASH, 1).append(path);
        ::free(cwd);
    }
}


}
}
