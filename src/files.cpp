#ifndef FILES
#define FILES

#include "io.cpp"

int makeFile(Disk *disk, std::string filePath, std::string content)
{
    FSHeader header = {};
    readHeader(disk, header);

    size_t size = content.length();

    disk->seekDisk(header.bitmapOffset, UNISEEK_BEG);

    uint8_t *bitmap = readBitmap(disk);

    size_t free_size;
    size_t start_block = header.rootDirOffset / 512 + 64;

    for (size_t i = 0; i < header.bitmapSizeBytes * 8; i++)
    {
        if (!checkBlockUsed(bitmap, i))
        {
            free_size++;
            if (i != 0)
            {
                if (checkBlockUsed(bitmap, i - 1))
                {
                    start_block = i + header.rootDirOffset / 512 + 64;
                }
            }
        }
        else
        {
            free_size = 0;
        }
        if (free_size >= (size + 511) / 512)
            break;
    }
    if (!(free_size >= (size + 511) / 512))
    {
        std::cerr << "Insufficient disk space in a row. Fragmentation will be in the future.";
        return -1;
    }

    char *charFilePath = (char *)malloc(96);

    memcpy(charFilePath, content.c_str(), content.length());

    split_cstr(charFilePath, '/');
    createDirEntry(charFilePath, start_block, size, 0);

    
    
    free(charFilePath);
}

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

// int readTextFile(Disk disk, const char* filePath, uint8_t*& buffer, uint32_t offset, uint32_t size)
//{
//     FSHeader header;
//     readHeader(disk, header);
//
//     for (size_t i = 0; i < 256; i++)
//     {
//
//         DirEntry entry;
//         if (!readDirEntry(disk, header.rootDirOffset + i * sizeof(DirEntry), entry))
//         {
//             std::cerr << "Magic is not correct" << std::endl;
//             return -1;
//         }
//
//         int nameLen = strlen(filePath);
//
//         if (std::memcmp(entry.name, filePath, nameLen) != 0)
//         {
//             continue;
//         }
//
//         break;
//     }
//
//     readDirEntry(disk, );
// }

// int readAllText(const char *diskPath, const char *filePath, uint8_t *&buffer)
//{
//     //readTextFile();
// }

#endif