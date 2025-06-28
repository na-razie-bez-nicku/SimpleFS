#include <fcntl.h>
#include <unistd.h>
#include <iostream>

int main()
{
    // std::cout << "Enter : ";
    int fd = open("disk.bin", O_WRONLY | O_CREAT);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    int blocks;

    std::cout << "Enter blocks of disk (1 block = 512 bytes): ";

    std::cin >> blocks;

    unsigned char buffer[512] = { 0x00 };

    for (size_t i = 0; i < blocks; i++)
    {
        write(fd, buffer, 512);
    }

    close(fd);
    return 0;
}
