#include <cstdint>

#pragma pack(push, 1) // wyłącza padding między polami

struct FSHeader {
    char     magic[8];          // "SIMPLEFS" (lub inny identyfikator)
    uint16_t version;           // wersja systemu plików (np. 1)
    uint32_t blockSize;         // rozmiar bloku w bajtach (np. 512)
    uint32_t blockCount;        // liczba bloków na nośniku
    uint32_t freeBlockCount;    // liczba wolnych bloków
    uint32_t bitmapOffset;      // offset (w bajtach) bitmapy alokacji
    uint32_t rootDirOffset;     // offset (w bajtach) katalogu głównego
    uint16_t maxFilenameLength; // maksymalna długość nazw plików
    uint8_t  reserved[32];      // miejsce na przyszłość (dopełnienie do 64 bajtów)
};

#pragma pack(pop)
