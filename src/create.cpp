#ifndef CREATE
#define CREATE

#include "disk.hpp"
#include <iostream>

void create(char *path)
{
    // char *file_name;
    // std::cout << "Enter file name (without extension): ";
    // std::cin >> file_name;

    uint64_t megabytes;

    std::cout << "Enter size of disk (MB): ";

    std::cin >> megabytes;

    Disk disk(path);

    if (!disk.openDisk(OPMD_CREAT | OPMD_WRONLY))
    {
        std::cerr << "Failed to open or create file: " << path << std::endl;
        return;
    }

    unsigned char buffer[1048576] = {0x00};

    for (size_t i = 0; i < megabytes; i++)
    {
        disk.writeDisk(buffer, 1048576);
    }

    disk.closeDisk();
}

void create(char *path, uint64_t size)
{
    // char *file_name;
    // std::cout << "Enter file name (without extension): ";
    // std::cin >> file_name;

    Disk disk(path);

    if (!disk.openDisk(OPMD_CREAT | OPMD_WRONLY))
    {
        std::cerr << "Failed to open or create file: " << path << std::endl;
        return;
    }

    unsigned char buffer[1048576] = {0x00};

    for (size_t i = 0; i < size; i++)
    {
        disk.writeDisk(buffer, 1048576);
    }

    disk.closeDisk();
}

#endif