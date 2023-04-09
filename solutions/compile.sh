
#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Usage: compile_cpp.sh [file_name]"
  exit 1
fi

file_name=$1
dir=`pwd`
cd ../helper
ls
g++ -c bit_reader.cpp -o bit_reader.o
g++ -c bit_writer.cpp -o bit_writer.o
cd $dir

g++ -std=c++17 -Wall $file_name.cpp ../helper/bit_reader.o ../helper/bit_writer.o -o ${file_name}.out
