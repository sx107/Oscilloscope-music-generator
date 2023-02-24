cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(osci)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system iostreams filesystem)

include_directories(${PROJECT_SOURCE_DIR}/include/;/usr/local/include;${Boost_INCLUDE_DIR})

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(osci main.cc ${sources} ${headers})
target_link_libraries(osci boost_system boost_filesystem boost_iostreams)