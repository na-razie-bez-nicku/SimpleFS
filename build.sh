if [ ! -d build ]; then
  mkdir build
fi

g++ -c src/main.cpp -o main.o
g++ -c src/disk.cpp -o disk.o

g++ main.o disk.o -o ./build/main

rm main.o
rm disk.o