set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

cmake_minimum_required(VERSION 3.13)
project(MP3Encoder)

include_directories(lame-3.100/include/)
link_directories(lame-3.100/lib)

add_executable(MP3Encoder src/main.cpp src/encoder.cpp src/encoder.h src/parallel_encoder.cpp src/parallel_encoder.h)

target_link_libraries(MP3Encoder libmp3lame.a -lpthread)
