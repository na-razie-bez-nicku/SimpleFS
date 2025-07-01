if [ ! -d build ]; then
  mkdir build
fi

g++ -c src/main.cpp -o main.o
g++ main.o -o ./build/main

rm main.o