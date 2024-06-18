cmake_minimum_required(VERSION 3.22)
project(cpp-structure LANGUAGES C CXX)

# Default Build Type to be Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build/)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++20 for dependencies and ABI compatibility
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
