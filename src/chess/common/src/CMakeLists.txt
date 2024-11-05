project(common)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../)

file(GLOB PROJECT_SRC *.cpp)

add_library(${PROJECT_NAME} ${PROJECT_SRC})
