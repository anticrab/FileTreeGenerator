cmake_minimum_required(VERSION 3.27)
project(FileTreeGenerator)

set(CMAKE_CXX_STANDARD 23)
add_subdirectory(json)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)


