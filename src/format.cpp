#ifndef FORMAT
#define FORMAT

#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdint.h>
#include "fs_header.cpp"
#include "disk.hpp"
#include "methods.cpp"

unsigned int calculateBitmapBlocks(uint64_t totalSizeBytes, uint32_t blockSize, uint32_t systemBlocksWithoutBitmap)
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
    return 0;
}

int format(Disk *disk)
{
    const int16_t blockSize = 512;

    uint64_t totalSize = getFileSize(disk->path);

    if (totalSize < 8192 * blockSize)
    {
        std::cerr << "Disk is to small. minimal size is 4MB" << std::endl;
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
    header.end[0] = 0x55;
    header.end[1] = 0xAA;

    // 512 bytes buffer (boot sector)
    uint8_t bootSector[512] = {0x00};
    memcpy(bootSector, &header, sizeof(header));

    // Save sector
    size_t written = disk->writeDisk(bootSector, sizeof(bootSector));
    if (written != sizeof(bootSector))
    {
        disk->closeDisk();
        return 1;
    }

    uint8_t *bitmap = new uint8_t[bitmapSizeBytes2];
    memset(bitmap, 0, bitmapSizeBytes2);

    disk->seekDisk(header.bitmapOffset, UNISEEK_BEG);
    disk->writeDisk(bitmap, bitmapSizeBytes2);
    return 0;
}

#endif