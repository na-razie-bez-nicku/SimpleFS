#ifndef CREATE
#define CREATE

#include "disk.hpp"
#include <iostream>

void create(Disk *diskPtr)
{
    // char *file_name;
    // std::cout << "Enter file name (without extension): ";
    // std::cin >> file_name;

    int blocks;

    std::cout << "Enter blocks of disk (1 block = 512 bytes): ";

    std::cin >> blocks;

    Disk disk = *diskPtr;

	disk.openDisk(OPMD_CREAT | OPMD_WRONLY);

    unsigned char buffer[512] = { 0x00 };

    for (size_t i = 0; i < blocks; i++)
    {
        disk.writeDisk(buffer, 512);
    }

    disk.closeDisk();
}

#endif