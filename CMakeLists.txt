cmake_minimum_required(VERSION 3.27)
project(FileTreeGenerator)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(json)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)


