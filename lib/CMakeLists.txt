cmake_minimum_required(VERSION 3.0)

project(Lib)

add_library(Serial SHARED Serial.cc Serial.h)
add_library(Tetromino SHARED Tetromino.cc Tetromino.h)