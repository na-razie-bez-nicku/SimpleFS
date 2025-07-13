#ifndef FS_HEADER
#define FS_HEADER

#include <cstdint>

#pragma pack(push, 1)

struct FSHeader {
    char     magic[8];          // "SIMPLEFS"
    uint8_t  pad0[2];           // 0x0000
    uint16_t version;           // FS Version
    uint8_t  pad1[4];           // 0x00000000
    uint32_t blockSize;         // block size in bytes 
    uint8_t  pad2[4];           // 0x00000000
    uint64_t blockCount;        // blocks amount on device1
    uint8_t  pad3[8];           // 0x0000000000000000
    uint64_t freeBlockCount;    // free blocks amount
    uint8_t  pad4[4];           // 0x00000000
    uint32_t bitmapOffset;      // allocation bitmap offset (in bytes)
    uint8_t  pad5[4];           // 0x00000000
    uint64_t rootDirOffset;     // root directory offset (in bytes)
    uint8_t  pad6[2];           // 0x0000
    uint16_t maxFilenameLength; // file name max
    uint8_t  pad7[2];           // 0x0000
    uint32_t bitmapSizeBytes;
    uint8_t  reserved[432];     // reserve
    uint8_t  end[2];
};

#pragma pack(pop)

#endif