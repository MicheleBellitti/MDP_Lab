
#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Usage: compile_cpp.sh [file_name]"
  exit 1
fi

file_name=$1
g++ $file_name.cpp -o ${file_name}.out
