cmake_minimum_required(VERSION 3.0.0)
project(fps-test VERSION 0.1.0)

# Configure compiler to use C++latest
set(CMAKE_CXX_STANDARD 23)

include(CTest)
enable_testing()

File(GLOB MyCppSources src/*.cpp)

add_executable(fps-test ${MyCppSources})

#add_compile_options(/NODEFAULTLIB:LIBCMT)
#add_compile_options(/NODEFAULTLIB:LIBCMTD)
#add_compile_options(/MTd)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(fps-test PRIVATE sfml-system sfml-network sfml-graphics sfml-window)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

if(CMAKE_GENERATOR MATCHES "Visual Studio") 
    add_custom_command(TARGET fps-test POST_BUILD
                       COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/$<CONFIGURATION>/Resources")
else()
    add_custom_command(TARGET fps-test POST_BUILD
                       COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/Resources")
endif()