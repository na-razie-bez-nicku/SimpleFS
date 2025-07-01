#include "io.cpp"

#pragma once

int readTextFile(int fd, DirEntry entry, uint8_t *&buffer, uint32_t offset, uint32_t size)
{
    uint32_t minimumSize = offset + size;

    uint32_t finalSize = size;

    if (entry.sizeBytes < minimumSize)
    {
        size = entry.sizeBytes - offset;
    }

    lseek(fd, entry.startBlock + size, SEEK_SET);

    read(fd, buffer, size);

    return finalSize;
}

int readTextFile(int fd, const char *filePath, uint8_t *&buffer, uint32_t offset, uint32_t size)
{
    FSHeader header;
    readHeader(fd, header);

    for (size_t i = 0; i < 256; i++)
    {

        DirEntry entry;
        if (!readDirEntry(fd, header.rootDirOffset + i * sizeof(DirEntry), entry))
        {
            std::cerr << "Magic is not correct" << std::endl;
            return;
        }

        int nameLen = strlen(filePath);

        if (std::memcmp(entry.name, filePath, nameLen) != 0)
        {
            continue;
        }

        break;
    }

    readDirEntry(fd, );
}

int readTextFile(const char *diskPath, const char *filePath, uint8_t *&buffer, uint32_t offset, uint32_t size)
{
}

int readAllText(const char *diskPath, const char *filePath, uint8_t *&buffer)
{

    readTextFile();
}