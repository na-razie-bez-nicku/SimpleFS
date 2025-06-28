#include <cstdint>

#pragma pack(push, 1)
struct DirEntry {
    char     name[32];     // nazwa pliku (null-terminated lub nie)
    uint32_t startBlock;   // numer pierwszego bloku z danymi pliku
    uint32_t sizeBytes;    // rozmiar pliku w bajtach
    uint8_t  type;         // 0 = plik, 1 = katalog
    uint8_t  reserved[23]; // padding do 64 bajtów
};
#pragma pack(pop)
