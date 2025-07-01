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
#endif
}