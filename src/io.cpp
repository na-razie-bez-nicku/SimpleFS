#ifndef IO
#define IO

#include <iostream>
#include <cstring>
#include <cstdint>
#include <vector>
#include "dir_entry.cpp"
#include "fs_header.cpp"
#include "methods.cpp"
#include "disk.hpp"

int printHeader(Disk disk)
{
    const int SECTOR_SIZE = 512;
    char buffer[SECTOR_SIZE];

    size_t bytesRead = disk.readDisk(buffer, SECTOR_SIZE);
    if (bytesRead != SECTOR_SIZE)
    {
        disk.closeDisk();
        return 1;
    }

    // Przekształć bajty do struktury
    FSHeader header;
    std::memcpy(&header, buffer, sizeof(FSHeader));

    // Sprawdź magic string
    if (std::memcmp(header.magic, "SIMPLEFS", 8) != 0)
    {
        std::cerr << "Invalid file system (magic mismatch)" << std::endl;
        return 1;
    }

    // Wyświetl dane
    std::cout << "✅ SIMPLEFS header loaded!" << std::endl;
    std::cout << "Version:             " << header.version << std::endl;
    std::cout << "Block size:          " << header.blockSize << " bytes" << std::endl;
    std::cout << "Total blocks:        " << header.blockCount << std::endl;
    std::cout << "Free blocks:         " << header.freeBlockCount << std::endl;
    std::cout << "Bitmap offset:       " << header.bitmapOffset << " bytes" << std::endl;
    std::cout << "Root dir offset:     " << header.rootDirOffset << " bytes" << std::endl;
    std::cout << "Max filename length: " << header.maxFilenameLength << " characters" << std::endl;
    std::cout << "Bitmap Size: " << header.bitmapSizeBytes << " bytes" << std::endl;

    return 0;
}

int printHeader(FSHeader header)
{
    // Wyświetl dane
    std::cout << "✅ SIMPLEFS header loaded!" << std::endl;
    std::cout << "Version:             " << header.version << std::endl;
    std::cout << "Block size:          " << header.blockSize << " bytes" << std::endl;
    std::cout << "Total blocks:        " << header.blockCount << std::endl;
    std::cout << "Free blocks:         " << header.freeBlockCount << std::endl;
    std::cout << "Bitmap offset:       " << header.bitmapOffset << " bytes" << std::endl;
    std::cout << "Root dir offset:     " << header.rootDirOffset << " bytes" << std::endl;
    std::cout << "Max filename length: " << header.maxFilenameLength << " characters" << std::endl;
    std::cout << "Bitmap Size: " << header.bitmapSizeBytes << " bytes" << std::endl;

    return 0;
}

int readHeader(Disk *disk, FSHeader &header)
{
    disk->seekDisk(0, UNISEEK_BEG);

    const int SECTOR_SIZE = 512;
    char buffer[SECTOR_SIZE];

    size_t bytesRead = disk->readDisk(buffer, SECTOR_SIZE);
    if (bytesRead != SECTOR_SIZE)
    {
        disk->closeDisk();
        return -1;
    }

    // Przekształć bajty do struktury
    std::memcpy(&header, buffer, sizeof(FSHeader));

    // Sprawdź magic string
    if (std::memcmp(header.magic, "SIMPLEFS", 8) != 0)
    {
        std::cerr << "Invalid file system (magic mismatch)" << std::endl;
        return -1;
    }

    return bytesRead;
}

uint8_t *readBitmap(Disk *disk)
{
    FSHeader header = {};

    readHeader(disk, header);

    uint32_t bitmapSizeBytes = (header.blockCount + 7) / 8;

    uint8_t *bitmap = new uint8_t[bitmapSizeBytes];

    disk->seekDisk(header.bitmapOffset, UNISEEK_BEG);
    disk->readDisk(bitmap, bitmapSizeBytes);

    return bitmap;
}

bool checkBlockUsed(uint8_t *bitmap, uint32_t blockIndex)
{
    return bitmap[blockIndex / 8] & (1 << (blockIndex % 8));
}

void setBlockUsed(uint8_t *bitmap, uint32_t blockIndex, bool used)
{
    uint32_t byteIndex = blockIndex / 8;
    uint8_t bitMask = 1 << (blockIndex % 8);

    if (used)
        bitmap[byteIndex] |= bitMask; // ustaw bit
    else
        bitmap[byteIndex] &= ~bitMask; // wyczyść bit
}

bool saveBitmapToDisk(Disk *disk, uint8_t *bitmap)
{
    disk->seekDisk(0, UNISEEK_BEG);

    FSHeader header = {};
    readHeader(disk, header);

    disk->seekDisk(header.bitmapOffset, UNISEEK_BEG);

    size_t written = disk->writeDisk(bitmap, header.bitmapSizeBytes);
    if (written != (size_t)header.bitmapSizeBytes)
    {
        disk->closeDisk();
        return false;
    }

    return true;
}

bool readRootDir(Disk *disk, uint32_t rootDirOffset, uint32_t rootDirSizeBytes, std::vector<DirEntry> &entries)
{
    disk->seekDisk(rootDirOffset, UNISEEK_BEG);

    std::vector<uint8_t> buffer(rootDirSizeBytes);
    size_t bytesRead = disk->readDisk(buffer.data(), rootDirSizeBytes);

    if (bytesRead != (size_t)rootDirSizeBytes)
    {
        std::cerr << "Failed to read full root directory\n";
        return false;
    }

    // Rozpakuj wpisy (zakładam, że DirEntry ma stały rozmiar)
    size_t entryCount = rootDirSizeBytes / sizeof(DirEntry);
    entries.resize(entryCount);
    memcpy(entries.data(), buffer.data(), rootDirSizeBytes);

    return true;
}

bool writeRootDir(Disk *disk, uint32_t rootDirOffset, const std::vector<DirEntry> &entries)
{
    disk->seekDisk(rootDirOffset, UNISEEK_BEG);

    size_t bytesWritten = disk->writeDisk(entries.data(), entries.size() * sizeof(DirEntry));

    if (bytesWritten != (size_t)(entries.size() * sizeof(DirEntry)))
    {
        std::cerr << "Failed to write full root directory\n";
        return false;
    }

    return true;
}

void updateBitmap(Disk *disk, DirEntry entry)
{
    FSHeader header = {};
    readHeader(disk, header);

    uint8_t *bitmap = readBitmap(disk);

    // uint64_t startBlock = entry.startBlock;
    uint64_t usedBlocks = (entry.sizeBytes + 511) / 512;
    // uint64_t endBlock = startBlock + usedBlocks;

    // for (size_t i = startBlock; i < endBlock; i++)
        // setBlockUsed(bitmap, i - header.rootDirOffset - 32, true);

    saveBitmapToDisk(disk, bitmap);
}

void createFile(Disk *disk, const char *filePath, const char *content)
{
    FSHeader header = {};
    readHeader(disk, header);

    bool created = false;

    for (size_t i = 0; i < 256; i++)
    {
        DirEntry entry = {};
        if (!readDirEntry(disk, header.rootDirOffset + i * sizeof(DirEntry), entry))
        {
            std::cerr << "Magic is not correct" << std::endl;
            return;
        }

        int nameLen = strlen(filePath);

        if (std::memcmp(entry.name, filePath, nameLen) != 0)
        {
            continue;
        }

        // disk->seekDisk(entry.startBlock * 512, UNISEEK_BEG);

        disk->writeDisk(content, entry.sizeBytes);
        created = true;

        std::cout << "File created." << std::endl;

        break;
    }

    if (!created)
    {
        std::cerr << "File not found in root dir" << std::endl;
    }

    // lseek(fd, entry.startBlock, SEEK_SET);

    // char *magic;
    // read(fd, magic, 4);
    // if (magic != "ENTR"){
    //     std::cerr << "Magic is not correct";
    //     return;
    // }

    // uint8_t *buffer;
    // read(fd, buffer, 96);
    // if()
}

#endif