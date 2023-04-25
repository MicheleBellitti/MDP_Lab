
#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Usage: compile_cpp.sh [file_name]"
  exit 1
fi

file_name=$1
dir=`pwd`
cd ../helper
ls
g++ -std=c++20 -c matrix.cpp -o matrix.o

cd $dir

g++ -std=c++20 -Wall $file_name.cpp ../helper/matrix.o -o ${file_name}.out
