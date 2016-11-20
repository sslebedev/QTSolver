cmake_minimum_required(VERSION 3.6)

### Project settings
set(PROJECT_NAME Solver)
project(${PROJECT_NAME})
message(WARNING "Project: ${PROJECT_NAME}")

add_compile_options(-std=gnu++11)
add_compile_options(-fvisibility=hidden)

add_definitions(-DBUILDING_DLL)

include_directories(../SolverInterface)
set(SOURCE_FILES solver.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

add_gtest(${PROJECT_NAME}_UnitTest test.cpp)
target_link_libraries(${PROJECT_NAME}_UnitTest SolverInterface)