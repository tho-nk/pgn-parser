cmake_minimum_required(VERSION 3.22)
project(pgn_parser)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/chess/)

add_subdirectory(chess)

set(PROJECT_SRC main.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SRC})

target_link_libraries(${PROJECT_NAME}
                        pthread
                        piece
                        move
                        common
                        )
