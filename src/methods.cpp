#ifndef METHODS
#define METHODS

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#endif
#include <string>
#include <cstring>
#include <vector>
#include <stdint.h>
#include <cctype>
#include <sstream>

inline bool exists(const std::string &name)
{
#ifdef _WIN32
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	if (GetFileAttributesExA(name.c_str(), GetFileExInfoStandard, &fileInfo))
	{
		return true; // File exists
	}
	else
	{
		return false; // File does not exist
	}
#else
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
#endif
}

uint64_t getFileSize(const std::string &filename)
{
#ifdef _WIN32
	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	if (GetFileAttributesExA(filename.c_str(), GetFileExInfoStandard, &fileInfo))
	{
		LARGE_INTEGER size;
		size.LowPart = fileInfo.nFileSizeLow;
		size.HighPart = fileInfo.nFileSizeHigh;
		return size.QuadPart; // Return file size in bytes
	}
	else
	{
		return 0; // File does not exist or error occurred
	}
#else
	struct stat buffer;
	if (stat(filename.c_str(), &buffer) == 0)
	{
		return buffer.st_size; // Return file size in bytes
	}
	else
	{
		return 0; // File does not exist or error occurred
	}
#endif
}

bool startsWith(const char *str, const char *prefix)
{
	return std::strncmp(str, prefix, std::strlen(prefix)) == 0;
}

std::vector<char *> split_cstr(char *str, char delimiter)
{
	char *token = strtok(str, &delimiter);
	std::vector<char *> tokens;

	while (token != nullptr)
	{
		tokens.push_back(token);
		token = strtok(nullptr, &delimiter);
	}

	return tokens;
}

std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string> parts;
	std::stringstream ss(str);
	std::string item;

	while (std::getline(ss, item, delimiter))
	{
		parts.push_back(item);
	}

	return parts;
}

bool containsLetter(const std::string &s)
{
	for (char c : s)
	{
		if (std::isalpha(static_cast<unsigned char>(c)))
		{
			return true;
		}
	}
	return false;
}

#endif