#ifndef FILES
#define FILES

#include "io.cpp"

int makeFile(Disk *disk, std::string filePath, std::string content)
{
    FSHeader header = {};
    readHeader(disk, header);

    size_t size = content.length();
    if (content == "\0")
        size = 512;

    disk->seekDisk(header.bitmapOffset, UNISEEK_BEG);

    uint8_t *bitmap = readBitmap(disk);

    size_t free_size = 0;
    size_t start_block = header.rootDirOffset / 512 + 128;

    for (size_t i = 0; i < header.bitmapSizeBytes * 8; i++)
    {
        if (!checkBlockUsed(bitmap, i))
        {
            free_size++;
            if (i != 0)
            {
                if (checkBlockUsed(bitmap, i - 1))
                {
                    start_block = i + header.rootDirOffset / 512 + 128;
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

    std::cout << free_size << "\n";

    bool fragmented = free_size < (size + 511) / 512;

    if (fragmented)
    {
        std::cerr << "Insufficient disk space in a row. Fragmentation will be in the future.";
        // return -1;

        std::vector<Run> frags = {};
        size_t frag_size = 0;
        for (size_t i = 0; i < header.bitmapSizeBytes * 8; i++)
        {
            if (!checkBlockUsed(bitmap, i))
            {
                if (i != 0)
                {
                    if (checkBlockUsed(bitmap, i - 1))
                    {
                        frags.push_back({i, 0});
                    }
                }
                else
                {
                    frags.push_back({i, 0});
                }
                frag_size++;
            }
            else
            {
                if (i != 0)
                {
                    if (!checkBlockUsed(bitmap, i - 1))
                    {
                        frags[frags.size() - 1].length = frag_size;
                        frag_size = 0;
                    }
                }
            }
        }

        std::vector<DirEntry> rootDir;

        std::cout << "ok\n";

        readRootDir(disk, header.rootDirOffset, 65536, rootDir);

        std::cout << "ok2\n";

        for (size_t i = 0; i < 256; i++)
        {
            if (memcmp(rootDir[i].magic, "ENTR", 4) != 0)
            {
                std::vector<DirEntry> dirEntry{createDirEntry(filePath.c_str(), frags, content.length(), 0)};
                writeRootDir(disk, header.rootDirOffset + i * sizeof(DirEntry), dirEntry);
                break;
            }
            if (i >= 255)
            {
                std::cerr << "Root directory is full!" << std::endl;
            }
        }

        size_t current_offset = disk->seekDisk(frags[0].offset * 512, UNISEEK_BEG) - header.rootDirOffset - 128 * 512;

        std::cout << frags[0].offset << "\n";
        std::cout << current_offset << "\n";

        for (size_t i = 0; i < frags.size(); i++)
        {
            Run frag = frags[i];

            for (size_t j = 0; j < frag.length; j++)
            {
                std::cout << current_offset / 512 + j << "\n";
                setBlockUsed(bitmap, current_offset / 512 + j, true);
            }

            current_offset = disk->seekDisk(frag.offset * 512, UNISEEK_CUR);
            std::cout << current_offset / 512 << ": " << frag.length << "\n";
            disk->writeDisk(content.substr(i * 512).c_str(), frag.length * 512);
            current_offset += frag.length * 512;

            // current_offset += frag.length;
        }

        disk->seekDisk(header.bitmapOffset, UNISEEK_BEG);

        saveBitmapToDisk(disk, bitmap);

        free(bitmap);

        return 0;
    }

    std::vector<DirEntry> rootDir = {};

    readRootDir(disk, header.rootDirOffset, 65536, rootDir);

    for (size_t i = 0; i < 256; i++)
    {
        if (memcmp(rootDir[i].magic, "ENTR", 4) != 0)
        {
            std::vector<DirEntry> dirEntry{createDirEntry(filePath.c_str(), start_block, content.length(), 0)};
            writeRootDir(disk, header.rootDirOffset + i * sizeof(DirEntry), dirEntry);
            break;
        }
        if (i >= 255)
        {
            std::cerr << "Root directory is full!" << std::endl;
        }
    }

    disk->seekDisk(header.bitmapOffset, UNISEEK_BEG);

    for (size_t i = 0; i < (size + 511) / 512; i++)
    {
        setBlockUsed(bitmap, start_block + i - header.rootDirOffset / 512 - 128, true);
    }

    saveBitmapToDisk(disk, bitmap);

    free(bitmap);

    if (content == "\0")
        return 0;

    disk->seekDisk(start_block * 512, UNISEEK_BEG);
    disk->writeDisk(content.c_str(), content.length());

    return 0;
}

int readTextFile(Disk *disk, DirEntry entry, char *&buffer, uint64_t offset, uint64_t size)
{
    uint64_t minimumSize = offset + size;

    uint64_t finalSize = size;

    if (entry.sizeBytes < minimumSize)
    {
        size = entry.sizeBytes - offset;
    }

    // std::cout << finalSize;

    // disk->seekDisk(entry.startBlock * 512 + offset, UNISEEK_BEG);

    disk->readDisk(buffer, finalSize);

    return finalSize;
}

int readTextFile(Disk *disk, const char *filePath, char *&buffer, uint64_t offset, uint64_t size)
{
    FSHeader header;
    readHeader(disk, header);

    for (size_t i = 0; i < 256; i++)
    {
        DirEntry entry;
        if (!readDirEntry(disk, header.rootDirOffset + i * sizeof(DirEntry), entry))
        {
            continue;
        }

        int nameLen = strlen(filePath);

        if (std::memcmp(entry.name, filePath, nameLen) == 0)
        {
            return readTextFile(disk, entry, buffer, offset, size);
        }
    }

    std::cerr << "File \"" << filePath << "\" doesn't exists" << std::endl;
    return -1;
}

int readAllText(Disk *disk, const char *filePath, char *&buffer)
{
    FSHeader header;
    readHeader(disk, header);

    for (size_t i = 0; i < 256; i++)
    {
        DirEntry entry;
        if (!readDirEntry(disk, header.rootDirOffset + i * sizeof(DirEntry), entry))
        {
            continue;
        }

        int nameLen = strlen(filePath);

        if (std::memcmp(entry.name, filePath, nameLen) == 0)
        {
            buffer = (char *)malloc(entry.sizeBytes + 1);
            buffer[entry.sizeBytes] = 0x00;
            return readTextFile(disk, entry, buffer, 0, entry.sizeBytes);
        }
    }

    std::cerr << "File \"" << filePath << "\" doesn't exists" << std::endl;
    return -1;
}

#endif