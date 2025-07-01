#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <sys/stat.h>
#include "fs_header.cpp"

uint32_t calculateBitmapBlocks(uint64_t totalSizeBytes, uint32_t blockSize, uint32_t systemBlocksWithoutBitmap)
{
    uint32_t totalBlocks = totalSizeBytes / blockSize;
    uint32_t availableBlocks = totalBlocks - systemBlocksWithoutBitmap;
    uint32_t lastResult = 0;

    while (true)
    {
        uint32_t bitmapSizeBytes = (availableBlocks + 7) / 8;
        uint32_t bitmapBlocks = (bitmapSizeBytes + blockSize - 1) / blockSize;

        uint32_t newAvailableBlocks = totalBlocks - systemBlocksWithoutBitmap - bitmapBlocks;

        if (newAvailableBlocks == availableBlocks || newAvailableBlocks == lastResult)
            return bitmapBlocks;

        lastResult = availableBlocks;
        availableBlocks = newAvailableBlocks;
    }
}

int format(char *path)
{
    const int16_t blockSize = 512;

    // char *path = strcat(rawPath, ".bin");

    struct stat fileStat;
    if (stat(path, &fileStat) == -1)
    {
        perror("stat");
        return 1;
    }

    uint64_t totalSize = fileStat.st_size;

    if (totalSize < 8192 * blockSize)
    {
        std::cerr << "Disk is to small. minimal size is 4MB";
        return 1;
    }

    uint32_t totalBlocks = totalSize / blockSize;


    uint64_t bitmapSizeBytes = (totalBlocks + 7) / 8;
    uint32_t bitmapBlockCount = (bitmapSizeBytes + blockSize - 1) / blockSize;
    
    uint32_t dataBlocks = totalBlocks - 72 - bitmapBlockCount;

    uint32_t bitmapSizeBytes2 = (dataBlocks + 7) / 8;

    uint32_t usedSystemBlocks = 72 + bitmapBlockCount;

    // Initialize header

    FSHeader header = {};
    memcpy(header.magic, "SIMPLEFS", 8);
    header.version = 1;
    header.blockSize = blockSize;
    header.blockCount = totalBlocks - usedSystemBlocks;
    header.freeBlockCount = totalBlocks - usedSystemBlocks;
    header.bitmapOffset = blockSize; // sector 1
    header.rootDirOffset = blockSize + calculateBitmapBlocks(totalSize, blockSize, 72) * blockSize;
    header.maxFilenameLength = 96;
    header.bitmapSizeBytes = bitmapSizeBytes2;

    // 512 bytes buffer (boot sector)
    uint8_t bootSector[512] = {0x00};
    memcpy(bootSector, &header, sizeof(header));
    bootSector[510] = 0x55;
    bootSector[511] = 0xAA;

    // Open file
    int fd = open(path, O_WRONLY, 0644);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    // Save sector
    ssize_t written = write(fd, bootSector, sizeof(bootSector));
    if (written != sizeof(bootSector))
    {
        perror("write");
        close(fd);
        return 1;
    }

    uint8_t *bitmap = new uint8_t[bitmapSizeBytes2];
    memset(bitmap, 0, bitmapSizeBytes2);

    lseek(fd, header.bitmapOffset, SEEK_SET);
    write(fd, bitmap, bitmapSizeBytes2);

    close(fd);
    return 0;
}