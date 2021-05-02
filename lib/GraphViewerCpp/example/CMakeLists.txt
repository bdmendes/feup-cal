cmake_minimum_required(VERSION 3.15)
project(example)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 14)

include_directories("${PROJECT_SOURCE_DIR}/../SFML/include")
link_directories("${PROJECT_SOURCE_DIR}/../SFML/lib")

add_subdirectory(.. build)

include_directories(../include)

add_executable(example main.cpp)

target_link_libraries(${PROJECT_NAME} graphviewer)
