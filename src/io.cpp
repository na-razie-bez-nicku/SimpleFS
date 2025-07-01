#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdint>
#include <vector>
#include "dir_entry.cpp"
#include "fs_header.cpp"
#include "methods.cpp"

int printHeader(const char *path)
{
    const int SECTOR_SIZE = 512;
    uint8_t buffer[SECTOR_SIZE];

    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    ssize_t bytesRead = read(fd, buffer, SECTOR_SIZE);
    if (bytesRead != SECTOR_SIZE)
    {
        perror("read");
        close(fd);
        return 1;
    }

    close(fd);

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

void readHeader(const char *path, FSHeader &header)
{
    const int SECTOR_SIZE = 512;
    uint8_t buffer[SECTOR_SIZE];

    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return;
    }

    ssize_t bytesRead = read(fd, buffer, SECTOR_SIZE);
    if (bytesRead != SECTOR_SIZE)
    {
        perror("read");
        close(fd);
        return;
    }

    close(fd);

    // Przekształć bajty do struktury=
    std::memcpy(&header, buffer, sizeof(FSHeader));

    // Sprawdź magic string
    if (std::memcmp(header.magic, "SIMPLEFS", 8) != 0)
    {
        std::cerr << "Invalid file system (magic mismatch)" << std::endl;
        return;
    }
}

void readHeader(int fd, FSHeader &header)
{
    const int SECTOR_SIZE = 512;
    uint8_t buffer[SECTOR_SIZE];

    if (fd < 0)
    {
        perror("open");
        return;
    }

    ssize_t bytesRead = read(fd, buffer, SECTOR_SIZE);
    if (bytesRead != SECTOR_SIZE)
    {
        perror("read");
        close(fd);
        return;
    }

    close(fd);

    // Przekształć bajty do struktury=
    std::memcpy(&header, buffer, sizeof(FSHeader));

    // Sprawdź magic string
    if (std::memcmp(header.magic, "SIMPLEFS", 8) != 0)
    {
        std::cerr << "Invalid file system (magic mismatch)" << std::endl;
        return;
    }
}

uint8_t *readBitmap(const char *path)
{
    FSHeader header = {};
    int fd = open(path, O_RDONLY);
    read(fd, &header, sizeof(header));

    uint32_t bitmapSizeBytes = (header.blockCount + 7) / 8;

    uint8_t *bitmap = new uint8_t[bitmapSizeBytes];

    lseek(fd, header.bitmapOffset, SEEK_SET);
    read(fd, bitmap, bitmapSizeBytes);

    close(fd);

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

bool saveBitmapToDisk(const char *path, uint8_t *bitmap, uint32_t bitmapSizeBytes, uint32_t bitmapOffset)
{
    int fd = open(path, O_WRONLY);
    if (fd < 0)
    {
        perror("open");
        return false;
    }

    if (lseek(fd, bitmapOffset, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        return false;
    }

    ssize_t written = write(fd, bitmap, bitmapSizeBytes);
    if (written != (ssize_t)bitmapSizeBytes)
    {
        perror("write");
        close(fd);
        return false;
    }

    close(fd);
    return true;
}

bool readRootDir(const char *diskPath, uint32_t rootDirOffset, uint32_t rootDirSizeBytes, std::vector<DirEntry> &entries)
{
    int fd = open(diskPath, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        return false;
    }

    if (lseek(fd, rootDirOffset, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        return false;
    }

    std::vector<uint8_t> buffer(rootDirSizeBytes);
    ssize_t bytesRead = read(fd, buffer.data(), rootDirSizeBytes);
    close(fd);

    if (bytesRead != (ssize_t)rootDirSizeBytes)
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

bool writeRootDir(const char *diskPath, uint32_t rootDirOffset, const std::vector<DirEntry> &entries)
{
    int fd = open(diskPath, O_WRONLY);
    if (fd < 0)
    {
        perror("open");
        return false;
    }

    if (lseek(fd, rootDirOffset, SEEK_SET) == -1)
    {
        perror("lseek");
        close(fd);
        return false;
    }

    ssize_t bytesWritten = write(fd, entries.data(), entries.size() * sizeof(DirEntry));
    close(fd);

    if (bytesWritten != (ssize_t)(entries.size() * sizeof(DirEntry)))
    {
        std::cerr << "Failed to write full root directory\n";
        return false;
    }

    return true;
}

void createFile(const char *diskPath, const char *filePath, const char *content)
{
    FSHeader header;
    readHeader(diskPath, header);

    int fd = open(diskPath, O_RDWR);

    bool created = false;

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

        lseek(fd, entry.startBlock * 512, SEEK_SET);

        write(fd, content, entry.sizeBytes);
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

    close(fd);
}