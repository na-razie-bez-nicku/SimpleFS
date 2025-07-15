#ifndef CREATE
#define CREATE

#include <fstream>
#include <iostream>

void create(std::string path)
{
    // char *file_name;
    // std::cout << "Enter file name (without extension): ";
    // std::cin >> file_name;

    uint64_t megabytes;

    std::cout << "Enter size of disk (MB): ";

    std::cin >> megabytes;

    std::ofstream img(path);

    if (img.bad())
    {
        std::cerr << "Failed to open or create file: " << path << std::endl;
        return;
    }

    char buffer[1048576] = {0x00};

    for (size_t i = 0; i < megabytes; i++)
    {
        img.write(buffer, 1048576);
    }

    img.close();
}

void create(std::string path, uint64_t size)
{
    // char *file_name;
    // std::cout << "Enter file name (without extension): ";
    // std::cin >> file_name;

    std::ofstream img(path);

    if (img.bad())
    {
        std::cerr << "Failed to open or create file: " << path << std::endl;
        return;
    }

    char buffer[1048576] = {0x00};

    for (size_t i = 0; i < size; i++)
    {
        img.write(buffer, 1048576);
    }

    img.close();
}

#endif