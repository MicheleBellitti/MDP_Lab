#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Usage: compile_y4m.sh [file_name]"
  exit 1
fi

file_name=$1
dir=`pwd`
cd utils
ls
g++ -std=c++20 -c pgm.cpp -o pgm.o
g++ -std=c++20 -c ppm.cpp -o ppm.o

cd $dir

g++ -std=c++20 -Wall $file_name.cpp utils/pgm.o utils/ppm.o -o ${file_name}
