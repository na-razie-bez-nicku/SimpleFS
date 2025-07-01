#include <cstdint>

#pragma once
#pragma pack(push, 1)

struct FSHeader {
    char     magic[8];          // "SIMPLEFS"
    uint16_t version;           // FS Version
    uint32_t blockSize;         // block size in bytes 
    uint64_t blockCount;        // blocks amount on device1
    uint64_t freeBlockCount;    // free blocks amount
    uint32_t bitmapOffset;      // allocation bitmap offset (in bytes)
    uint32_t rootDirOffset;     // root directory offset (in bytes)
    uint16_t maxFilenameLength; // file name max 
    uint32_t bitmapSizeBytes;
    uint8_t  reserved[20];      // reserve
} __attribute__((packed));

#pragma pack(pop)
