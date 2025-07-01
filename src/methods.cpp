#include <sys/stat.h>
#include <string>
#include <cstring>
#include <iostream>

#pragma once

inline bool exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

bool startsWith(const char* str, const char* prefix) {
    return std::strncmp(str, prefix, std::strlen(prefix)) == 0;
}

void split_cstr(char* str, char delimiter) {
    char* token = strtok(str, &delimiter);

    while (token != nullptr) {
        std::cout << token << "\n";
        token = strtok(nullptr, &delimiter);
    }
}
