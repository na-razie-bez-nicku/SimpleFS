#ifndef DIR_ENTRY
#define DIR_ENTRY

#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#include "disk.hpp"

#pragma pack(push, 1)
struct DirEntry
{
    char magic[4];        // "ENTR"
    uint8_t pad0[2];      // 0x0000
    char name[128];       // file name
    uint8_t pad1[2];      // 0x0000
    uint64_t sizeBytes;   // file size in bytes
    uint8_t pad3[2];      // 0x000000
    uint8_t type;         // 0 = file, 1 = directory
    uint8_t runlist[109]; // runlist with reserve
};

struct Run
{
    uint64_t offset, length;
};
#pragma pack(pop)

uint8_t minBytesForNumber(uint64_t value)
{
    uint8_t bytes = 0;
    do
    {
        bytes++;
        value >>= 8;
    } while (value != 0);

    return bytes;
}

size_t createRunlist(std::vector<Run> runs, uint8_t *&byteRuns)
{
    size_t length = 1 + minBytesForNumber(runs[0].offset) + minBytesForNumber(runs[0].length);
    byteRuns = (uint8_t *)malloc(length);

    byteRuns[0] = minBytesForNumber(runs[0].offset) * 16 + minBytesForNumber(runs[0].length);

    for (int i = 0; i < minBytesForNumber(runs[0].offset); i++)
    {
        byteRuns[i + 1] = (runs[0].offset >> (8 * i)) & 0xFF;
    }

    for (int i = 0; i < minBytesForNumber(runs[0].length); i++)
    {
        byteRuns[i + minBytesForNumber(runs[0].offset) + 1] = (runs[0].length >> (8 * i)) & 0xFF;
    }

    std::cout << "dobra\n";

    for (size_t i = 1; i < runs.size(); i++)
    {
        std::cout << i << '\n';

        size_t currentRunLength = 1 + minBytesForNumber(runs[i].offset) + minBytesForNumber(runs[i].length);
        length += currentRunLength;

        uint8_t *temp = (uint8_t *)realloc(byteRuns, length);
        if (temp == nullptr)
        {
            free(byteRuns);
            return -1;
        }

        byteRuns = temp;

        byteRuns[length - 1] = minBytesForNumber(runs[i].offset) * 16 + minBytesForNumber(runs[i].length);

        for (int j = 0; j < minBytesForNumber(runs[i].offset - runs[i - 1].offset - runs[i - 1].length); j++)
        {
            byteRuns[j + 1] = ((runs[i].offset - runs[i - 1].offset - runs[i - 1].length) >> (8 * j)) & 0xFF;
        }

        for (int j = 0; j < minBytesForNumber(runs[i].length); j++)
        {
            byteRuns[j + minBytesForNumber(runs[i].length)] = (runs[i].length >> (8 * j)) & 0xFF;
        }
    }

    uint8_t *temp = (uint8_t *)realloc(byteRuns, length++);
    temp[length - 1] = 0x00;

    return length;
}

DirEntry createDirEntry(const char *filename, uint64_t startBlock, uint64_t sizeBytes, uint8_t type)
{
    DirEntry entry;
    std::memset(&entry, 0, sizeof(entry));
    memcpy(entry.magic, "ENTR", 4);
    std::strncpy(entry.name, filename, sizeof(entry.name) - 1);
    entry.sizeBytes = sizeBytes;
    entry.type = type;

    uint8_t headerHigh = minBytesForNumber(startBlock);
    uint8_t headerLow = minBytesForNumber((sizeBytes + 511) / 512);

    uint8_t *byteRuns = (uint8_t *)malloc(1 + headerHigh + headerLow);

    byteRuns[0] = headerHigh * 16 + headerLow;

    for (int i = 0; i < headerHigh; i++)
    {
        byteRuns[i + 1] = (startBlock >> (8 * i)) & 0xFF;
    }

    for (int i = 0; i < headerLow; i++)
    {
        byteRuns[i + headerHigh + 1] = (startBlock >> (8 * i)) & 0xFF;
    }

    byteRuns[headerHigh + headerLow] = 0x00;

    memcpy(entry.runlist, byteRuns, 1 + headerHigh + headerLow);

    free(byteRuns);

    return entry;
}

DirEntry createDirEntry(const char *filename, std::vector<Run> runs, uint64_t sizeBytes, uint8_t type)
{
    DirEntry entry;
    std::memset(&entry, 0, sizeof(entry));
    memcpy(entry.magic, "ENTR", 4);
    std::strncpy(entry.name, filename, sizeof(entry.name) - 1);
    entry.sizeBytes = sizeBytes;
    entry.type = type;

    uint8_t *byteRuns;
    size_t runlistLength = createRunlist(std::vector<Run>(runs), byteRuns);

    memcpy(entry.runlist, byteRuns, runlistLength);

    free(byteRuns);

    return entry;
}

bool readDirEntry(Disk *disk, uint64_t offset, DirEntry &entry)
{
    // przesuń wskaźnik na odpowiedni offset (np. rootDirOffset + index * sizeof(DirEntry))
    disk->seekDisk(offset, UNISEEK_BEG);

    // czytaj dane binarne do struktury
    if (disk->readDisk(&entry, sizeof(DirEntry)) != sizeof(DirEntry))
        return false;

    // sprawdź magic string
    return std::memcmp(entry.magic, "ENTR", 4) == 0;
}

#endif