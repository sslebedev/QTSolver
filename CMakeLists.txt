cmake_minimum_required(VERSION 3.6)

### Functions
function(add_gtest test_project_name files)
    add_executable(${test_project_name} ${files})
    target_link_libraries(${test_project_name} gtest gtest_main)
endfunction(add_gtest)

### Project settings
set(PROJECT_NAME QTSolver)
project(${PROJECT_NAME})
message(WARNING "Project: ${PROJECT_NAME}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(SolverInterface)
add_subdirectory(Solver)