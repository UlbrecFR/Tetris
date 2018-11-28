cmake_minimum_required(VERSION 3.0)

project(Lib)

add_library(Tetromino SHARED Tetromino.cc Tetromino.h)
add_library(Serializer SHARED Serializer.cc Serializer.h)