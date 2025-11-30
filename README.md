# Assignment-1: Time Travelling File System

## Overview

This project implements various data structures including Max Heap, Nodes, Linked Lists, and Hash maps and supports operations using these structures.

The project is written in **C++17**.

## Project Structure

The project contains separate files for the definition and implementation of classes TreeNodes, Nodes and MaxHeaps.
The main.cpp file contains the classes File and SystemStat. 

**Files structure**
Assignment-1/
├── main.cpp          # Entry point of the program
├── MaxHeap.cpp       # Implementation of Max Heap
├── TreeNode.cpp      # Implementation of TreeNode class
├── Node.cpp          # Node implementation (for linked list/tree)
├── LinkedList.cpp    # Implementation of Linked List
├── build.sh          # Script to compile the project
└── README.md         # Project instructions

There are the respective .h files for MaxHeap, Node, TreeNode and LinkedList as well. 


## Operations

There are multiple operations that can be done with files in this program. These commands are to be typed into `stdin` and entered. 

I have implemented the functions exactly as required in the description of the problem, so I am not including them here. I have added some additional operations that made it easier for me to test this code as I went about building it.

1. $\text{VERSION} <\text{filename}> [\text{num}]$
	- Here you can quickly look up a past version of a file, by referring to its version_id.
2. $\text{T\_VER <filename>}$
	- This displays the total versions that exist of that particular file
3. $\text{C\_VER <filename>}$
	- This displays the current version of file, the one that active_version points to. Note that this is zero indexed. The root file's version_id $= 0$
4. $\text{FILENAMES}$
	- This displays the names of all the files that exist in the program
5. $\text{END}$
	- Terminates the program 

## Data Structures

TreeNode is the unit of the tree of our file versions, implemented with pointers. 

We use a MaxHeap to track the biggest trees in our program, across different Files. 

To efficiently look up different versions of our files (various TreeNodes), we use a Hashmap. I used the idea of chaining to build my Hashmap. I encapsulated my TreeNode into a structure Node so that I can build a LinkedList. The Hashmap is then implemented as a vector of LinkedLists. 

## Build instructions

#### Requirements

- g++ compiler (supporting C++17)
- Linux or macOS terminal (or any environment supporting Bash scripts)
#### Build Instructions


1. Open a terminal and navigate to the project folder:

```bash
cd "/path/to/Assignment-1/"
```

2. Run the terminal

```bash
chmod +x build.sh
```

3. Compile the project

```bash
./build.sh
```

3. Run the Program

```bash
./program
```


Done by:
Abhineet Milind More
2024EE30810