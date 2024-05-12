# Introduction

**This project is aimed at building software for parsing pgn chess file.**

It requires:

- CMake 3.22
- GCC 12.1 and C++20 standard
- Ubuntu 22.04 or the Docker image `iainttho/ubuntu:millennium` based on Ubuntu 22.04.

# Build and Run

**Running the Docker Container**

The image `iainttho/ubuntu:millennium` comes with preinstalled required packages:
- gdb, valgrind, pytest


Start/Stop the Container:

- `docker run -d --name mlp-ubuntu iainttho/ubuntu:millennium`
- `docker stop mlp-ubuntu`


**Building, Testing, and Running**

Enter the container:

- `docker exec -it mlp-ubuntu bash`
- `cd`
- `git clone https://nk-tho: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@github.com/tho-nk/mlp-ha.git`
- `cd mlp-ha`
  
Scripts are available to build, test the project:

- `./compile.sh`
    - The executable mlp-ha is found in either ./build/ or ./pytest/ folder
  
- `./test.sh`

# Notes
I am still working on developing this parser.
