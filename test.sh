#!/bin/bash

# Define the directory and project variables






# Create the build directory if it doesn't exist
mkdir -p build

# Navigate into the build directory
cd build || { echo "Failed to navigate to build directory"; exit 1; }

# Run cmake to configure the project
cmake  ../src || { echo "CMake configuration failed"; exit 1; }


# Run make to build the project
make || { echo "Build failed"; exit 1; }

cd ..

cd bin

#./main -i ../examples/ex01.cfg -S -P 0-999 -b -r 200-204

./main -i ../examples/ex01.cfg -P 0-999 -b -r 105 -o nosso.txt

./main -b -i ../examples/ex01.cfg -o teste.txt


meld nosso.txt teste.txt
