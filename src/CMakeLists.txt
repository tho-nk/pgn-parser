cmake_minimum_required(VERSION 3.22)
project(mlp-ha)


add_subdirectory(pgn-parsing/src)

set(PROJECT_SRC main.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SRC})

target_link_libraries(${PROJECT_NAME}
                        pthread
                        pgn-parsing)
