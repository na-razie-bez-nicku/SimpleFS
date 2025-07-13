#include "format.cpp"
#include "create.cpp"
#include "io.cpp"
#include "methods.cpp"
#include <map>
#include <fstream>

int main(int argc, char *argv[])
{
    bool running = true;

    std::map<std::string, Disk> mounted_disks;

    std::string selected_disk = "/";

    // if (!exists("disk.bin"))
    // {
    //     create("disk.bin");

    //     disk.openDisk(OPMD_RDWR);

    //     format(&disk);

    //     disk.seekDisk(0, UNISEEK_BEG);
    // }
    // else
    // {
    //     disk.openDisk(OPMD_RDWR);
    // }

    char *charCommand;

    while (running)
    {
        std::cout << "SimpleFS:" << selected_disk << "$ ";

        std::string command;
        getline(std::cin, command);

        if (!containsLetter(command))
        {
            std::cout << "Please enter command! \"" << command << "\" is not a valid command" << std::endl;
            continue;
        }

        charCommand = (char *)malloc(command.length() + 1);

        memcpy(charCommand, command.c_str(), command.length());

        charCommand[command.length()] = '\0';

        std::vector<char *> args = split_cstr(charCommand, ' ');

        if (strcmp(args[0], "mkfile") == 0)
        {
            
        }
        else if (strcmp(args[0], "select") == 0)
        {
            if (args.size() == 1)
            {
                selected_disk = "/";
            }
            else
            {
                if (mounted_disks.count(std::string(args[1])) != 0)
                    selected_disk = "/" + std::string(args[1]);
                else
                    std::cout << "Disk \"" << args[1] << "\" not found" << std::endl;
            }
        }
        else if (strcmp(args[0], "create") == 0)
        {
            if (args.size() < 3)
            {
                std::cout << "To few arguments:" << std::endl
                          << "create <path> <size>" << std::endl
                          << "path - Path to new disk image" << std::endl
                          << "size - Size of image" << std::endl;

                continue;
            }

            if (!exists(args[1]))
                create(args[1], std::atoi(args[2]));
            else
                std::cout << "disk image \"" << args[1] << "\" already exists";
        }
        else if (strcmp(args[0], "mount") == 0)
        {
            if (args.size() < 3)
            {
                std::cout << "To few arguments:" << std::endl
                          << "mount <diskpath> <mountname>" << std::endl
                          << "diskpath - Path to disk" << std::endl
                          << "mountname - Name for mounted disk." << std::endl;
                continue;
            }
            if (mounted_disks.count(std::string(args[2])) != 0)
            {
                std::cout << "ERROR: disk with name \"" << args[2] << "\" is already mounted. Try with another name" << std::endl;
                continue;
            }
            if (!exists(args[1]))
            {
                std::cout << "disk \"" << args[1] << "\" doesn't exists";
            }
            Disk disk(args[1]);
            mounted_disks.insert_or_assign(args[2], disk);
            mounted_disks.at(std::string(args[2])).openDisk(OPMD_RDWR);
            
            std::cout << "Disk has been mounted" << std::endl;
        }
        else if (strcmp(args[0], "format") == 0)
        {
            if (args.size() < 2)
            {
                std::cout << "To few arguments:" << std::endl
                          << "format <disk>" << std::endl
                          << "disk - Disk name given in the \"mount\" command" << std::endl;
                continue;
            }
            if (mounted_disks.count(std::string(args[1])) != 0)
            {
                format(&mounted_disks.at(std::string(args[1])));
                std::cout << "Disk formated!" << std::endl;
            }
            else
                std::cout << "Disk \"" << args[1] << "\" not found" << std::endl;
        }
        else if (strcmp(args[0], "exit") == 0)
        {
            running = false;
            for (auto disk : mounted_disks)
            {
                disk.second.closeDisk();
            }
        }

        free(charCommand);
    }

    return 0;
}