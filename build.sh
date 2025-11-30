#!/bin/bash
# Build script for Assignment-1

# Exit on any error
set -e

# Compiler and flags
CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra -g"

# Source files
SOURCES="main.cpp MaxHeap.cpp TreeNode.cpp Node.cpp LinkedList.cpp"

# Output executable
OUTPUT="program"

# Compile
$CXX $CXXFLAGS $SOURCES -o $OUTPUT

echo "Build successful. Run with ./program"




