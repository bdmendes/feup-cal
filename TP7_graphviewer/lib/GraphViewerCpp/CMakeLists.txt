cmake_minimum_required(VERSION 3.10)

project(graphviewer)

if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
    set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()

if(DEFINED WIN32)
    add_subdirectory(SFML)
endif()

include_directories(SFML/include)
link_directories(SFML/lib)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

include_directories(include)

add_library(graphviewer STATIC
    src/graphviewer.cpp
    src/node.cpp
    src/edge.cpp
    src/lines.cpp
    src/fpsmonitor.cpp
)

target_compile_options(graphviewer PRIVATE ${CMAKE_CXX_LIB})
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
if (NOT DEFINED WIN32)
    target_link_libraries(${PROJECT_NAME} pthread)
    target_link_libraries(${PROJECT_NAME} X11)
endif()
