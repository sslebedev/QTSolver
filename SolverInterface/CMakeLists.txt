cmake_minimum_required(VERSION 3.6)

### Project settings
set(PROJECT_NAME SolverInterface)
project(${PROJECT_NAME})
message(WARNING "Project: ${PROJECT_NAME}")

set(CMAKE_CXX_FLAGS " -std=c++11")

set(SOURCE_FILES solver_wrap.cpp)
add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} dl)