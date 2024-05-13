# Introduction

**This project is aimed at building software for parsing pgn chess file based on SAN (Standard Algebraic Notation).**

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

# Features
This parser supports the PGN format based on : <br> http://www.saremba.de/chessgml/standards/pgn/pgn-complete.htm

- The movetext is based on SAN (Standard Algebraic Notation)
- Disambiguation is supported by checking `VerifyIfKingBeingCheck` before accepting a move
- PGN file format error detection
- Output format:
  - The state of the board at the end of the game, in the form of an 8x8 grid.
  - Initial state  
```
bR|bN|bB|bQ|bK|bB|bN|bR
bP|bP|bP|bP|bP|bP|bP|bP
  |  |  |  |  |  |  |  
  |  |  |  |  |  |  |  
  |  |  |  |  |  |  |  
  |  |  |  |  |  |  |  
wP|wP|wP|wP|wP|wP|wP|wP
wR|wN|wB|wQ|wK|wB|wN|wR
```

  - Input
```
1. e4 e6 2. e5 f6 3. exf6 gxf6 4. Ke2 h6 5. Ke3 h5 6. Ke4 e5 7. Nh3 d6 8. Ng5
Be6 9. Nf3 Bh3 10. Nc3 Bxg2 11. Na4 b6 12. Nc5 b5 13. Nd3 b4 14. Nxe5 b3 1/2-1/2
```
   - Output :
```
bR|bN|  |bQ|bK|bB|bN|bR
bP|  |bP|  |  |  |  |  
  |  |  |bP|  |bP|  |  
  |  |  |  |wN|  |  |bP
  |  |  |  |wK|  |  |  
  |bP|  |  |  |wN|  |  
wP|wP|wP|wP|  |wP|bB|wP
wR|  |wB|wQ|  |wB|  |wR
```

-   Runtime polymorphism in `class Move;`
-   Static polymorphism with `std::variant`, `std::visit` and `CRTP (Curiously Recurring Template Pattern)` in `class BasePiece;`

# Notes
I am still working on developing this parser.

-   Add Iterative mode
-   Support Forward-Backward interaction
-   Take in account tags and comment
-   Improve performace and readability
-   Cover more file format error
-   Improve test coverage



