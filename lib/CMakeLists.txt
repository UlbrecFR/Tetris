cmake_minimum_required(VERSION 3.0)

project(Lib)

add_library(Tetris SHARED Tetromino.cc Tetromino.h Serializer.cc Serializer.h Deserializer.cc Deserializer.h Serial.h)
