#include <cstdint>
#include <cstring>
#include <unistd.h>

#pragma pack(push, 1)
struct DirEntry {
    char     magic[4];     // "ENTR"
    char     name[96];     // file name
    uint64_t startBlock;   // first block of file data position
    uint64_t sizeBytes;    // file size in bytes
    uint8_t  type;         // 0 = file, 1 = directory
    uint8_t  reserved[11]; // reserve
};
#pragma pack(pop)

DirEntry createDirEntry(const char* filename, uint32_t startBlock, uint32_t sizeBytes, uint8_t type) {
    DirEntry entry;
    std::memset(&entry, 0, sizeof(entry));
    memcpy(entry.magic, "ENTR", 4);
    std::strncpy(entry.name, filename, sizeof(entry.name) - 1);
    entry.startBlock = startBlock;
    entry.sizeBytes = sizeBytes;
    entry.type = type;

    return entry;
}

bool readDirEntry(int fd, off_t offset, DirEntry &entry) {
    // przesuń wskaźnik na odpowiedni offset (np. rootDirOffset + index * sizeof(DirEntry))
    if (lseek(fd, offset, SEEK_SET) < 0)
        return false;

    // czytaj dane binarne do struktury
    if (read(fd, &entry, sizeof(DirEntry)) != sizeof(DirEntry))
        return false;

    // sprawdź magic string
    return std::memcmp(entry.magic, "ENTR", 4) == 0;
}