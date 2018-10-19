
cmake_minimum_required(VERSION 3.0)

project (Tetris CXX)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11 -O2 -g")

find_package(Boost REQUIRED system thread)
include_directories(${BOOST_INCLUDE_DIRS})

find_package(gf REQUIRED)

add_subdirectory(lib)

add_executable(TetrisServer
  TetrisServer.cc
  TetrisServer.h
)

target_link_libraries(TetrisServer ${Boost_LIBRARIES})

add_executable(TetrisClient
  TetrisClient.cc
  TetrisClient.h
)

target_link_libraries(TetrisClient ${Boost_LIBRARIES} Tetromino)
