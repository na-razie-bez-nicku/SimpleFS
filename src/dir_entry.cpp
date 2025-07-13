#ifndef DIR_ENTRY
#define DIR_ENTRY

#include <cstdint>
#include <cstring>
#include "disk.hpp"

#pragma pack(push, 1)
struct DirEntry
{
    char magic[4];       // "ENTR"
    uint8_t pad0[2];     // 0x0000
    char name[96];       // file name
    uint8_t pad1[2];     // 0x0000
    uint64_t startBlock; // first block of file data position
    uint8_t pad2[3];     // 0x0000
    uint64_t sizeBytes;  // file size in bytes
    uint8_t pad3[3];     // 0x0000
    uint8_t type;        // 0 = file, 1 = directory
    uint8_t reserved[1]; // reserve
};
#pragma pack(pop)

DirEntry createDirEntry(const char *filename, uint64_t startBlock, uint64_t sizeBytes, uint8_t type)
{
    DirEntry entry;
    std::memset(&entry, 0, sizeof(entry));
    memcpy(entry.magic, "ENTR", 4);
    std::strncpy(entry.name, filename, sizeof(entry.name) - 1);
    entry.startBlock = startBlock;
    entry.sizeBytes = sizeBytes;
    entry.type = type;

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