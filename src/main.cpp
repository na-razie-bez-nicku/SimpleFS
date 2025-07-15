#include "format.cpp"
#include "create.cpp"
#include "io.cpp"
#include "methods.cpp"
#include "files.cpp"
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

        auto args = split(command, ' ');

        if (args[0].compare("cat") == 0)
        {
            if (args.size() < 2)
            {
                std::cout << "To few arguments:" << std::endl
                          << "cat <path>" << std::endl
                          << "path - Path to file" << std::endl;

                continue;
            }
            if (selected_disk == "/")
            {
                std::cout << "Disk is not selected! Select disk with \"select\" command" << std::endl;
                continue;
            }
            if (mounted_disks.count(selected_disk.substr(1)) != 0)
            {
                // std::string buffer;
                char *buffer = nullptr;
                if (readAllText(&mounted_disks.at(selected_disk.substr(1)), args[1].c_str(), buffer) != -1)
                {
                    std::cout << buffer << std::endl;
                    free(buffer);
                }
            }
        }
        else if (args[0].compare("mkfile") == 0)
        {
            if (args.size() < 2)
            {
                std::cout << "To few arguments:" << std::endl
                          << "mkfile <path> [content]" << std::endl
                          << "path - Path to new file" << std::endl
                          << "content - Content of new file" << std::endl;

                continue;
            }
            if (selected_disk == "/")
            {
                std::cout << "Disk is not selected! Select disk with \"select\" command" << std::endl;
                continue;
            }
            if (mounted_disks.count(selected_disk.substr(1)) != 0)
                if (args.size() < 3)
                    makeFile(&mounted_disks.at(selected_disk.substr(1)), args[1], "");
                else
                {
                    std::string content = args[2];

                    for (size_t i = 3; i < args.size(); i++)
                    {
                        content += " ";
                        content += args[i];
                    }

                    makeFile(&mounted_disks.at(selected_disk.substr(1)), args[1], content);
                }
        }
        else if (args[0].compare("select") == 0)
        {
            if (args.size() == 1)
            {
                selected_disk = "/";
            }
            else
            {
                if (mounted_disks.count(args[1]) != 0)
                    selected_disk = "/" + args[1];
                else
                    std::cout << "Disk \"" << args[1] << "\" not found" << std::endl;
            }
        }
        else if (args[0].compare("create") == 0)
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
                create(args[1], std::atoi(args[2].c_str()));
            else
                std::cout << "disk image \"" << args[1] << "\" already exists" << std::endl;
        }
        else if (args[0].compare("mount") == 0)
        {
            if (args.size() < 3)
            {
                std::cout << "To few arguments:" << std::endl
                          << "mount <diskpath> <mountname>" << std::endl
                          << "diskpath - Path to disk" << std::endl
                          << "mountname - Name for mounted disk." << std::endl;
                continue;
            }
            if (mounted_disks.count(args[2]) != 0)
            {
                std::cout << "ERROR: disk with name \"" << args[2] << "\" is already mounted. Try with another name" << std::endl;
                continue;
            }
            if (!exists(args[1]))
            {
                std::cout << "disk \"" << args[1] << "\" doesn't exists" << std::endl;
                continue;
            }
            Disk disk(args[1]);
            mounted_disks.insert(std::make_pair(args[2], disk));
            mounted_disks.at(args[2]).openDisk(OPMD_RDWR);

            std::cout << "Disk has been mounted" << std::endl;
        }
        else if (args[0].compare("format") == 0)
        {
            if (args.size() < 2)
            {
                std::cout << "To few arguments:" << std::endl
                          << "format <disk>" << std::endl
                          << "disk - Disk name given in the \"mount\" command" << std::endl;
                continue;
            }
            if (mounted_disks.count(args[1]) != 0)
            {
                format(&mounted_disks.at(args[1]));
                std::cout << "Disk formated!" << std::endl;
            }
            else
                std::cout << "Disk \"" << args[1] << "\" not found" << std::endl;
        }
        else if (args[0].compare("exit") == 0)
        {
            running = false;
            for (auto disk : mounted_disks)
            {
                disk.second.closeDisk();
            }
        }
    }

    return 0;
}