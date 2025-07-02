#ifdef _WIN32
  #include <windows.h>
#else
  #include <fcntl.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/stat.h>
#endif
#include <stdint.h>

#define UNISEEK_BEG 0
#define UNISEEK_CUR 1
#define UNISEEK_END 2

#define OPMD_RDONLY 0
#define OPMD_WRONLY 1
#define OPMD_RDWR 2

class Disk {
public:
  Disk(const char* path);
  ~Disk(const char* path);

  bool openDisk(uint8_t mode);
  ssize_t readDisk(void* buffer, size_t size);
  ssize_t writeDisk(const void* buffer, size_t size);
  void seekDisk(uint64_t offset, uint8_t from); // 0 - begin; 1 - current; 2 - end (reverse)
  void closeDisk();

private:
	char* path;
#ifdef _WIN32
  HANDLE handle = INVALID_HANDLE_VALUE;
#else
  int fd = -1;
#endif
};
