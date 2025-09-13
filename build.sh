#!/usr/bin/env sh

if [ ! -d build ]; then
  mkdir build
fi

g++ -c ./src/main.cpp -o main.o
g++ -c ./src/disk.cpp -o disk.o

g++ main.o disk.o -o ./build/simplefs

rm main.o
rm disk.o

x86_64-w64-mingw32-g++ -c ./src/main.cpp -o main.o
x86_64-w64-mingw32-g++ -c ./src/disk.cpp -o disk.o

x86_64-w64-mingw32-g++ main.o disk.o -o ./build/simplefs.exe -static-libstdc++ -static-libgcc

rm main.o
rm disk.o