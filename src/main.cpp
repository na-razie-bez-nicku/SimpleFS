#define _AMD64_ 1

#include "format.cpp"
#include "create.cpp"
#include "io.cpp"
#include "methods.cpp"
#include <fstream>

int main(int argc, char *argv[])
{
    Disk disk("C:\\SimpleDisks\\disk.bin");

    if (!exists("C:\\SimpleDisks\\disk.bin"))
    {
        create(&disk);

        disk.openDisk(OPMD_RDWR);

        format(&disk);
    }
    else {
        disk.openDisk(OPMD_RDWR);
    }

    // if(argc > 1){
    //     if(argv[1] == "format"){
    //         if(argc > 2)
    //             format(argv[2]);
    //         else
    //             format("disk");
    //     }
    //     else if(argv[1] == "create"){
    //         create();
    //     }
    //     else{
    //         std::cout << "command " << argv[1] << " doesn't exists\n" << argc;

    //     }
    // }

    printHeader(&disk);

    FSHeader header = {};

    readHeader(&disk, header);

    // uint8_t *bitmap = readBitmap("disk.bin");

    // saveBitmapToDisk("disk.bin", bitmap, header.bitmapSizeBytes, header.bitmapOffset);

    writeRootDir(&disk, header.rootDirOffset, std::vector<DirEntry>{
                                                       createDirEntry("Hello.txt", 68, 25, 0),  // Hello World from SimpleFS
                                                       createDirEntry("Hello2.txt", 69, 32, 0), // Second Hello World from SimpleFS
                                                       createDirEntry("Lorem Ipsum.txt", 70, 3284, 0),
                                                       createDirEntry("Moby Dick; Or, The Whale.txt", 77, 1276288, 0),
                                                       createDirEntry("The History of Herodotus — Volume 1.txt", 2570, 915847, 0),
                                                   });

    createFile(&disk, "Hello.txt", "Hello World from SimpleFS");
    createFile(&disk, "Hello2.txt", "Second Hello World from SimpleFS");
    createFile(&disk, "Lorem Ipsum.txt", R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras interdum facilisis urna sit amet luctus. Cras luctus efficitur quam, non porta turpis malesuada id. Mauris tempor molestie cursus. Nam gravida, nisl vitae rutrum bibendum, est justo finibus lacus, a ullamcorper justo felis et sapien. Proin bibendum finibus enim. Sed sed urna maximus, sodales lacus eu, condimentum quam. Ut pharetra lobortis varius. Nunc at augue vestibulum, sagittis justo sit amet, ultricies purus.

Donec feugiat porttitor risus, vel accumsan ligula porttitor eget. Praesent gravida, ipsum in aliquet venenatis, nisi orci eleifend tellus, sit amet ullamcorper lacus lorem hendrerit sapien. Nam in libero a ex sagittis molestie vitae eu lacus. Suspendisse potenti. Morbi placerat diam eu interdum venenatis. Praesent tincidunt risus arcu, non elementum risus viverra sed. Nunc dignissim luctus aliquam. Etiam facilisis elit placerat mauris finibus sodales. Quisque accumsan malesuada ornare. Suspendisse vel pharetra lacus. Donec ultricies mauris ut nisi maximus, ut cursus neque suscipit. Sed vel laoreet risus. Duis ac nibh sem. Aenean neque ex, blandit vel risus et, semper posuere sem. Sed ipsum mi, mattis et enim sit amet, egestas semper ante. Nunc pharetra libero nec facilisis lobortis.

Nulla fermentum aliquam est, nec suscipit dolor eleifend ac. Quisque mi metus, venenatis quis egestas sit amet, sollicitudin porta augue. Integer lobortis in tortor a lacinia. Pellentesque vitae euismod est. Vivamus feugiat fermentum turpis ac congue. Etiam ullamcorper ultricies eros, eget bibendum nisi aliquet quis. Nam ex tellus, dignissim et interdum in, interdum nec felis. Ut neque nulla, vehicula ac molestie id, finibus in enim. Sed sed ultricies ligula.

Phasellus eros leo, accumsan quis lacinia sed, congue at sapien. Donec imperdiet sapien nec euismod ornare. Nulla et malesuada mauris, vitae viverra nulla. In elit nibh, ullamcorper ornare hendrerit sed, pulvinar vitae velit. Pellentesque quis justo suscipit, placerat leo at, cursus justo. Praesent mattis suscipit eros sit amet dapibus. Nullam posuere metus dui, quis hendrerit ligula rhoncus vel. Quisque efficitur urna a nisi posuere imperdiet. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi finibus elementum ex, vel vehicula enim placerat vitae. Sed augue libero, consectetur pulvinar nibh ac, ornare pretium est. Sed urna nunc, gravida ac vulputate quis, egestas id velit. Proin finibus ornare arcu, eget tristique augue rutrum id. Vivamus commodo sem in diam vulputate, id rhoncus leo fermentum. Sed ligula est, suscipit ac leo in, hendrerit aliquam orci.

Interdum et malesuada fames ac ante ipsum primis in faucibus. Curabitur et magna vehicula, suscipit ipsum eu, vestibulum magna. Etiam nec cursus augue. Praesent sollicitudin venenatis nisi in porta. Donec sed aliquet dolor. Donec non pellentesque lorem. Morbi luctus leo at velit condimentum imperdiet. Mauris eget mi libero. Suspendisse nec purus vitae ligula aliquam efficitur. Pellentesque pellentesque finibus ante a semper. Ut molestie eget nunc in tincidunt. Cras mattis suscipit quam, ac suscipit massa placerat sollicitudin. Donec rhoncus tellus interdum, semper libero non, hendrerit lacus. Aenean vehicula tortor velit, quis vulputate purus rutrum id.)");

    disk.closeDisk();

    return 0;
}