#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include "fs_header.cpp"

int main() {
    // Przygotuj nagłówek
    FSHeader header = {};
    memcpy(header.magic, "SIMPLEFS", 8);
    header.version = 1;
    header.blockSize = 512;
    header.blockCount = 8386551;
    header.freeBlockCount = 8386551;
    header.bitmapOffset = 512;         // sektor 1
    header.rootDirOffset = 512 + 4096; // sektor 9
    header.maxFilenameLength = 32;

    // Bufor 512 bajtów (boot sektor)
    uint8_t bootSector[512] = { 0x00 };
    memcpy(bootSector, &header, sizeof(header));
    bootSector[510] = 0x55;
    bootSector[511] = 0xAA;

    // Otwórz plik
    int fd = open("disk.bin", O_WRONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Zapisz sektor
    ssize_t written = write(fd, bootSector, sizeof(bootSector));
    if (written != sizeof(bootSector)) {
        perror("write");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

// int main() {
//     int fd = open("disk.bin", O_RDWR);
//     if (fd < 0) {
//         perror("open");
//         return 1;
//     }

//     // char buf[128];
//     // ssize_t bytesRead = read(fd, buf, sizeof(buf));
//     // if (bytesRead < 0) {
//     //     perror("read");
//     //     close(fd);
//     //     return 1;
//     // }

//     // std::cout << "Wczytano " << bytesRead << " bajtów.\n";

//     unsigned char write_buffer[512] = { 0x53, 0x49, 0x4D, 0x50, 0x4C, 0x45, 0x46, 0x53 };
//     write(fd, write_buffer, 512);

//     close(fd);
//     return 0;
// }
