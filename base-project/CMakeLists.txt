cmake_minimum_required(VERSION 3.0.0)
project(tetris-like VERSION 0.1.0)

include(CTest)
enable_testing()

File(GLOB MyCppSources src/*.cpp)

add_executable(tetris-like ${MyCppSources})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
