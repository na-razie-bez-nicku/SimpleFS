#include <iomanip>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>

int main()
{
    int fd = open("/dev/nvme0n1", O_RDONLY);

    lseek64(fd, (loff_t)1543337982 * 512, SEEK_SET);

    unsigned char buffer[1024];
    read(fd, buffer, 1024);

    std::ofstream result("nwm_xd.bin", std::ios::binary);
    result.write(reinterpret_cast<char *>(buffer), 1024);

    result.close();

    // for (size_t i = 0; i < 512; i++)
    //     std::cout
    //         << std::hex << std::setw(2) << std::setfill('0') << (0xff & buffer[i]) << ' ';

    // std::cout << "\n\n";

    // for (size_t i = 512; i < 1024; i++)
    //     std::cout
    //         << std::hex << std::setw(2) << std::setfill('0') << (0xff & buffer[i]) << ' ';

    // std::cout << '\n';

    close(fd);

    return 0;
}