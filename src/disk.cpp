#ifndef FILEAPI_CPP
#define FILEAPI_CPP

#include "disk.hpp"

Disk::Disk(std::string path)
{
    this->path = path;
}

Disk::~Disk()
{
    closeDisk(); // automatyczne zamykanie pliku
}

bool Disk::openDisk(unsigned int mode)
{
#ifdef _WIN32
    if (mode & OPMD_CREAT)
        handle = CreateFile(path.c_str(), mode, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    else
        handle = CreateFile(path.c_str(), mode, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    return handle == INVALID_HANDLE_VALUE;
#else
    fd = open(path.c_str(), mode, 0644);
    return fd >= 0;
#endif
}

size_t Disk::readDisk(void *buffer, size_t size)
{
#ifdef _WIN32
    DWORD bytesRead;
    BOOL success = ReadFile(handle, buffer, size, &bytesRead, NULL);
    if (success)
    {
        return static_cast<size_t>(bytesRead);
        ; // Return number of bytes read
    }
    else
        return SIZE_MAX; // Error reading file
#else
    return read(fd, buffer, size);
#endif
}

size_t Disk::writeDisk(const void *buffer, size_t size)
{
#ifdef _WIN32
    DWORD bytesWritten;
    BOOL success = WriteFile(handle, buffer, size, &bytesWritten, NULL);
    if (success)
    {
        return static_cast<size_t>(bytesWritten);
        ; // Return number of bytes read
    }
    else
        return SIZE_MAX; // Error reading file
#else
    return write(fd, buffer, size);
#endif
}

void Disk::flushDisk()
{
#ifdef _WIN32
    FlushFileBuffers(handle);
#else
    fsync(fd);
#endif
}

void Disk::seekDisk(uint64_t offset, uint8_t from)
{
#ifdef _WIN32
    LARGE_INTEGER li;
    li.QuadPart = offset; // offset
    switch (from)
    {
    case UNISEEK_BEG:
        SetFilePointerEx(handle, li, NULL, FILE_BEGIN);
        break;
    case UNISEEK_CUR:
        SetFilePointerEx(handle, li, NULL, FILE_CURRENT);
        break;
    case UNISEEK_END:
        SetFilePointerEx(handle, li, NULL, FILE_END);
        break;
    }
#else
    lseek(fd, offset, from);
#endif
}

void Disk::closeDisk()
{
#ifdef _WIN32
    CloseHandle(handle);
#else
    close(fd);
#endif
}

#endif