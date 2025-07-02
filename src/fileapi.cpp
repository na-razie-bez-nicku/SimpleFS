#include "fileapi.hpp"

bool Disk::openDisk(const char *path, uint8_t mode)
{
#ifdef _WIN32
    handle = CreateFile()
#else
    switch (mode)
    {
    case OPMD_RDONLY:
        fd = open(path, O_RDONLY);
        break;
    case OPMD_WRONLY:
        fd = open(path, O_WRONLY);
        break;
    case OPMD_RDWR:
        fd = open(path, O_RDWR);
        break;
    }
    return fd >= 0;
#endif
}

ssize_t Disk::readDisk(void *buffer, size_t size)
{
#ifdef _WIN32

#else
    return read(fd, buffer, size);
#endif
}

ssize_t Disk::writeDisk(const void *buffer, size_t size)
{
#ifdef _WIN32

#else
    return write(fd, buffer, size);
#endif
}

void Disk::seekDisk(uint64_t offset, uint8_t from)
{
#ifdef _WIN32

#else
    switch (from)
    {
    case UNISEEK_BEG:
        lseek(fd, offset, SEEK_SET);
        break;
    case UNISEEK_CUR:
        lseek(fd, offset, SEEK_CUR);
        break;
    case UNISEEK_END:
        lseek(fd, offset, SEEK_END);
        break;
    }
#endif
}

void Disk::closeDisk()
{
#ifdef _WIN32

#else
    close(fd);
#endif
}