cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall -g")

# set the project name
project(algorithms)

# add the executable
add_executable(${PROJECT_NAME} tester.cc)

