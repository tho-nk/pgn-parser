# Introduction

**This project is aimed at building software for parsing pgn chess file based on SAN (Standard Algebraic Notation).**

It requires:

- CMake 3.22
- GCC 12.1 and C++20 standard
- Ubuntu 22.04 or the Docker image `iainttho/ubuntu:pgn` based on Ubuntu 22.04.

# Build and Run

**Running the Docker Container**

The image `iainttho/ubuntu:pgn` comes with preinstalled required packages:
- gdb, valgrind, pytest


Start/Stop the Container:
- `docker-compose up -d`
- `docker-compose down --remove-orphans`

**Building, Testing, and Running**

Enter the container:

- `docker exec -it pgn_ubuntu bash`
- `cd && cd pgn-parser`
  
Scripts are available to build, test the project:

- `./compile.sh`
    - The executable pgn_parser is found in either ./build/ or ./pytest/ folder
    - `cd pytest`
    - `./pgn_parser <file_path>`

- `./test.sh`

# Features
This parser supports the PGN format based on : <br> https://en.wikipedia.org/wiki/Portable_Game_Notation

- The movetext is based on SAN (Standard Algebraic Notation)
- Disambiguation is supported by checking `VerifyIfKingBeingCheck_` before accepting a move
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
-   Dispatch using `std::variant`, `std::visit` and `CRTP (Curiously Recurring Template Pattern)` in `template <typename ConcretePiece> class BasePiece;`

# Notes
I am still working on developing this parser. Please feel free to provide any feedback or suggestions.

-   Add Iterative mode
-   Support Forward-Backward interaction
-   Process tags and comment
-   Improve performace and readability
-   Cover more file format error
-   Improve test coverage
-   Handle error
-   Overload piece swap
-   Handle Color (White/Blacl) more elegant
-   Pull Request!

