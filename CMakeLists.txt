cmake_minimum_required(VERSION 3.5)
project(MP3Encoder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MP3Encoder src/main.cpp src/Encoder.cpp src/Encoder.hpp src/ParallelEncoder.cpp src/ParallelEncoder.hpp)
target_link_libraries(MP3Encoder mp3lame pthread)

# TODO Build and run on windows :/