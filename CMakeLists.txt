cmake_minimum_required(VERSION 3.10)

# set the project name
project(algorithms)

# add the executable
add_executable(${PROJECT_NAME} tester.cc)

target_include_directories(${PROJECT_NAME}
    PRIVATE datastructures)
