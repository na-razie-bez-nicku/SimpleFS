#ifndef FILES
#define FILES

#include "io.cpp"

int readTextFile(Disk disk, DirEntry entry, uint8_t *&buffer, uint32_t offset, uint32_t size)
{
    uint32_t minimumSize = offset + size;

    uint32_t finalSize = size;

    if (entry.sizeBytes < minimumSize)
    {
        size = entry.sizeBytes - offset;
    }

    disk.seekDisk(entry.startBlock + size, UNISEEK_BEG);

    disk.readDisk(buffer, size);

    return finalSize;
}

//int readTextFile(Disk disk, const char* filePath, uint8_t*& buffer, uint32_t offset, uint32_t size)
//{
//    FSHeader header;
//    readHeader(disk, header);
//
//    for (size_t i = 0; i < 256; i++)
//    {
//
//        DirEntry entry;
//        if (!readDirEntry(disk, header.rootDirOffset + i * sizeof(DirEntry), entry))
//        {
//            std::cerr << "Magic is not correct" << std::endl;
//            return -1;
//        }
//
//        int nameLen = strlen(filePath);
//
//        if (std::memcmp(entry.name, filePath, nameLen) != 0)
//        {
//            continue;
//        }
//
//        break;
//    }
//
//    readDirEntry(disk, );
//}

//int readAllText(const char *diskPath, const char *filePath, uint8_t *&buffer)
//{
//    //readTextFile();
//}

#endif