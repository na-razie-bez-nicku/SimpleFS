if [ ! -d build ]; then
  mkdir build
fi

g++ -c src/FS/main.cpp -o main.o
g++ main.o -o ./build/main

g++ -c src/create_disk.cpp -o create.o
g++ create.o -o ./build/create

rm main.o
rm create.o