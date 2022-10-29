#include <asm-generic/errno-base.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>
#include <cassert>
#include <cerrno>
#include <cstdio>
#include <dirent.h>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <mutex>

#include "file.h"
#include "except.h"

using namespace std;

namespace dios::foundation
{

static std::vector<string> *deleteFileNames = NULL;
static mutex mtx;
static const char FORWARD_SLASH = '/';
static const char TILDE = '~';
static const char *DEFAULT_TEMP_DIR = "/tmp/";

const char LinuxFileSystem::LINUX_PATH_SEPARATOR
{ FORWARD_SLASH };

static bool fileExists(const char *path)
{
    struct stat sb;
    return ::stat(path, &sb) == 0;
}

const FileSystem *FileSystem::DefaultFileSystem
{ new LinuxFileSystem() };

void deleteFileOnExitHandler(void)
{
    for (std::vector<string>::iterator iter = deleteFileNames->begin(); iter != deleteFileNames->end(); ++iter)
        ::unlink((*iter).c_str());
    delete deleteFileNames;
}

FileException::FileException(const char *funcName, int lineNumber, const std::string &message) :
        Exception(funcName, lineNumber, message)
{
}

FileException::~FileException() throw ()
{
}

LinuxFileSystem::LinuxFileSystem()
{
}

LinuxFileSystem::~LinuxFileSystem()
{
}

bool LinuxFileSystem::touch(const char *path) const
{
    ofstream ofs(path);
    if (false == ofs.is_open())
        return false;
    ofs.close();
    return true;
}

bool LinuxFileSystem::createDirectories(const char *absolutePath) const
{
    string path(absolutePath);
    string::size_type pos = 0;
    while ((pos = path.find_first_of(FORWARD_SLASH, pos + 1)) != string::npos)
    {
        if (createDirectory(path.substr(0, pos).c_str()) == false)
            return false;
    }
    return createDirectory(absolutePath);
}

bool LinuxFileSystem::createDirectory(const char *path) const
{
    if (fileExists(path))
        return true;
    mode_t mode = S_IRWXU | S_IRWXG | S_IROTH;
    // It may seem redundant to use both calls but mkdir seems to ignore any mode
    // setting other than owner.
    return ::mkdir(path, 0) == 0 && ::chmod(path, mode) == 0;
}

string LinuxFileSystem::createTempFilename(const uint8_t *prefix, const uint8_t *suffix, const uint8_t *dirPath) const
{
    ostringstream oss;

    const char *dirName = (dirPath == NULL) ? DEFAULT_TEMP_DIR : (const char*) dirPath;
    oss << dirName << prefix << rand() << suffix;
    return oss.str();
}

const char* LinuxFileSystem::homeDir() const
{
    const char *homedir = NULL;
    if ((homedir = ::getenv("HOME")) == NULL)
        homedir = ::getpwuid(::getuid())->pw_dir;
    return homedir;
}

bool LinuxFileSystem::remove(const string &path) const
{
    return ::remove(path.c_str()) == 0;
}

bool LinuxFileSystem::rename(const std::string &oldPath, const std::string &newPath) const
{
    return ::rename(oldPath.c_str(), newPath.c_str()) == 0;
}

bool LinuxFileSystem::copy(const std::string &fromPath, const std::string &toPath) const
{
    string command("cp ");
    command.append(fromPath).append(" ");
    command.append(toPath);
    errno = 1;
    return ::system(command.c_str()) == 0;
}

string LinuxFileSystem::currentWorkingDirectory() const
{
    return string(::get_current_dir_name());
}

void LinuxFileSystem::normalizedPath(const string &path, string &returnPath) const
{
    if (path.size() > 0 && path.at(0) == FORWARD_SLASH)
    {
        returnPath.append(path);
    } else if (path.size() > 0 && path.at(0) == TILDE)
    {
        returnPath = path;
        returnPath.replace(0, 1, homeDir());
    } else
    {
        char *cwd = ::get_current_dir_name();
        returnPath.append(cwd);
        if (path.size() > 0)
            returnPath.append((char*) &FORWARD_SLASH, 1).append(path);
        ::free(cwd);
    }
}

File::File() :
        _prefixLength(0)
{
}

File::File(const char *path, const char *filename) :
        _prefixLength(0)
{
    string fn(path);
    FileSystem::DefaultFileSystem->normalizedPath(fn, _path);
    _path.append(&FORWARD_SLASH, 1).append(filename);
    _prefixLength = _path.find_last_of(FORWARD_SLASH);
}

File::File(const char *filename) :
        _prefixLength(0)
{
    string fn(filename);
    FileSystem::DefaultFileSystem->normalizedPath(fn, _path);
    _prefixLength = _path.find_last_of(FORWARD_SLASH);
}

File::File(const std::string &filename) :
        _prefixLength(0)
{
    FileSystem::DefaultFileSystem->normalizedPath(filename, _path);
    _prefixLength = _path.find_last_of(FORWARD_SLASH);
}

File::File(const File &otherFile) :
        _path(otherFile._path), _prefixLength(otherFile._prefixLength)
{
}

File::~File()
{
}

const char* File::name() const
{
    return &_path.c_str()[_prefixLength + 1];
}

bool File::touch() const
{
    return FileSystem::DefaultFileSystem->touch(absolutePath());
}

const char* File::absolutePath() const
{
    return _path.c_str();
}

const std::string File::parent() const
{
    return _path.substr(0, _prefixLength);
}

File File::parentFile() const
{
    return File(parent());
}

bool File::renameTo(const File &file)
{
    return FileSystem::DefaultFileSystem->rename(_path.c_str(), file._path.c_str());
}

bool File::mkdirs() const
{
    return FileSystem::DefaultFileSystem->createDirectories(parent().c_str());
}

bool File::copyTo(const File &toFile)
{
    return FileSystem::DefaultFileSystem->copy(_path.c_str(), toFile._path.c_str());
}

File& File::operator=(const File &otherFile)
{
    _path = otherFile._path;
    _prefixLength = otherFile._prefixLength;
    return *this;
}

void File::deleteOnExit() const
{
    if (deleteFileNames == NULL)
    {
        unique_lock<mutex> lock(mtx);
        // It may look odd to check again, but another thread might
        // be doing the same thing when we do this.
        if (deleteFileNames == NULL)
        {
            deleteFileNames = new std::vector<string>();
            ::atexit(deleteFileOnExitHandler);
        }
    }
    deleteFileNames->push_back(_path);
}

bool File::exists() const
{
    return fileExists(_path.c_str());
}

time_t File::modificationDate() const
{
    return modificationDate(_path.c_str());
}

int64_t File::size() const
{
    struct stat sb;
    return (stat(_path.c_str(), &sb) == -1) ? -1 : sb.st_size;
}

bool File::isDirectory() const
{
    struct stat sb;
    int32_t status = stat(_path.c_str(), &sb);
    if (status == -1)
    {
        if (errno == ENOENT)
            return false;
        else
            THROW_NAMED_EXCEPTION(FileException, ::strerror(errno));
    }
    return S_ISDIR(sb.st_mode);
}

bool File::isExecutable() const
{
    struct stat sb;
    int32_t status = stat(_path.c_str(), &sb);
    if (status == -1)
    {
        if (errno == ENOENT)
            return false;
        else
            THROW_NAMED_EXCEPTION(FileException, ::strerror(errno));
    }
    //todo: enhance so we only check relevant bits, if we own file or are group, or other...
    return ((sb.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0);
}

bool File::list(vector<std::string> &returnList, int (*filter)(const struct dirent*), int (*compare)(const struct dirent**, const struct dirent**)) const
{

    struct dirent **namelist;
    int n;

    n = ::scandir(_path.c_str(), &namelist, filter, compare);
    if (n < 0)
        return false;
    else
    {
        while (n--)
        {
            returnList.push_back(namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }
    return true;
}

bool File::list(vector<std::string> &returnList, int (*filter)(const struct dirent*)) const
{
    return list(returnList, filter, NULL);
}

bool File::list(vector<std::string> &returnList) const
{
    return list(returnList, NULL);
}

bool File::remove() const
{
    return FileSystem::DefaultFileSystem->remove(_path);
}

File File::createTempFile(const char *prefix, const char *suffix)
{
    return createTempFile(prefix, suffix, NULL);
}

File File::createTempFile(const char *prefix, const char *suffix, File *dir)
{
    assert(prefix != NULL);
    assert(suffix != NULL);
    return File(createTempFilename(prefix, suffix, dir));
}

string File::createTempFilename(const char *prefix, const char *suffix)
{
    return createTempFilename(prefix, suffix, NULL);
}

string File::createTempFilename(const char *prefix, const char *suffix, File *dir)
{
    assert(prefix != NULL);
    assert(suffix != NULL);
    const char *dirPath = (dir == NULL) ? NULL : dir->name();
    return FileSystem::DefaultFileSystem->createTempFilename((const uint8_t*) prefix, (const uint8_t*) suffix, (const uint8_t*) dirPath);
}

time_t File::modificationDate(const char *path)
{
    struct stat sb;
    stat(path, &sb);
    return sb.st_mtim.tv_sec;
}

bool File::exists(const string &path)
{
    return fileExists(path.c_str());
}

bool File::exists(const char *path)
{
    return fileExists(path);
}


Directory::Directory() {
    _pathname = FileSystem::DefaultFileSystem->currentWorkingDirectory();
}

Directory::Directory(const char* pathname) : _pathname(pathname) {
    if (::strcmp(".", pathname) == 0) {
        _pathname = FileSystem::DefaultFileSystem->currentWorkingDirectory();
    }
}

Directory::~Directory() {
}

bool Directory::mkdirs() const {
    if (_pathname.size() > 0) {
        std::string absolutePath;
        if (_pathname[0] != FileSystem::DefaultFileSystem->pathSeparator()) {
            string curDir = FileSystem::DefaultFileSystem->currentWorkingDirectory();
            absolutePath.append(curDir).push_back(FileSystem::DefaultFileSystem->pathSeparator());
            absolutePath.append(_pathname);
        } else
            absolutePath = _pathname;
        return FileSystem::DefaultFileSystem->createDirectories(absolutePath.c_str());
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


} /* namespace dios */
