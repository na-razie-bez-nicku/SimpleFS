#include "fileapi.hpp"

Disk::Disk(const char* path) {
    this->path = path;
}

bool Disk::openDisk(uint8_t mode)
{
#ifdef _WIN32
    switch (mode) {
	case OPMD_RDONLY:
		handle = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case OPMD_WRONLY:
		handle = CreateFile(path, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        break;
	case OPMD_RDWR:
		handle = CreateFile(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        break;
    }
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
    DWORD bytesRead;
    BOOL success = ReadFile(hFile, buffer, size, &bytesRead, NULL);
	if (success) {
		return bytesRead; // Return number of bytes read
	}
    else
        return -1; // Error reading file
#else
    return read(fd, buffer, size);
#endif
}

ssize_t Disk::writeDisk(const void *buffer, size_t size)
{
#ifdef _WIN32
    DWORD bytesWritten;
    BOOL success = WriteFile(hFile, buffer, size, &bytesWritten, NULL);
    if (success) {
        return bytesRead; // Return number of bytes read
    }
    else
        return -1; // Error reading file
#else
    return write(fd, buffer, size);
#endif
}

void Disk::seekDisk(uint64_t offset, uint8_t from)
{
#ifdef _WIN32
    LARGE_INTEGER li;
    li.QuadPart = offset;  // offset
    switch (from)
    {
    case UNISEEK_BEG:
        SetFilePointerEx(hFile, li, NULL, FILE_BEGIN);
        break;
    case UNISEEK_CUR:
        SetFilePointerEx(hFile, li, NULL, FILE_CURRENT);
        break;
    case UNISEEK_END:
        SetFilePointerEx(hFile, li, NULL, FILE_END);
        break;
    }
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
    CloseHandle(hFile);
#else
    close(fd);
#endif
}