#ifndef FILEAPI_HPP
#define FILEAPI_HPP

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include <stdint.h>
#include <limits>
#include <cstring>
#include <string>

#define UNISEEK_BEG 0
#define UNISEEK_CUR 1
#define UNISEEK_END 2

#ifdef _WIN32
#define OPMD_RDONLY 0x80000000
#define OPMD_WRONLY 0x40000000
#define OPMD_RDWR OPMD_RDONLY | OPMD_WRONLY
#else
#define OPMD_RDONLY 0
#define OPMD_WRONLY 1
#define OPMD_RDWR 2
#endif
#define OPMD_CREAT 100

class Disk
{
public:
	Disk() = default;
	Disk(std::string path);
	/*explicit Disk(const char* p) : path(p) {}
	const char* path;*/
	~Disk();

	// Disk(const Disk &) = delete;
	// Disk &operator=(const Disk &) = delete;

	/*Disk(const Disk&) = delete;
	Disk& operator=(const Disk&) = delete;

	Disk(Disk&&) = default;
	Disk& operator=(Disk&&) = default;*/

	std::string path;

	bool openDisk(uint8_t mode);
	size_t readDisk(void *buffer, size_t size);
	size_t writeDisk(const void *buffer, size_t size);
	void flushDisk();
	size_t seekDisk(uint64_t offset, uint8_t from); // 0 - begin; 1 - current; 2 - end (reverse)
	void closeDisk();

private:
#ifdef _WIN32
	HANDLE handle = INVALID_HANDLE_VALUE;
#else
	int fd = -1;
#endif
};

#endif