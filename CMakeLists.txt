
cmake_minimum_required(VERSION 3.0)

project (Tetris CXX)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -O2 -g")

find_package(Boost REQUIRED regex thread chrono)
include_directories(${BOOST_INCLUDE_DIRS})

add_executable(TetrisServer
  TetrisServer.cc
)

target_link_libraries(TetrisServer ${Boost_LIBRARIES})

add_executable(TetrisClient
  TetrisClient.cc
)

target_link_libraries(TetrisClient ${Boost_LIBRARIES})
