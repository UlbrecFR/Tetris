cmake_minimum_required(VERSION 3.0)

project(Lib)

add_library(TetrisClientLib SHARED Tetromino.cc Tetromino.h Serializer.cc Serializer.h Deserializer.cc Deserializer.h Grid.cc Grid.h Serial.h GameArea.cc GameArea.h DisplayGame.cc DisplayGame.h Controls.h)

add_library(TetrisServerLib SHARED Tetromino.cc Tetromino.h Serializer.cc Serializer.h Deserializer.cc Deserializer.h Grid.cc Grid.h Serial.h)