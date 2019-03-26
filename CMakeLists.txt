set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

cmake_minimum_required(VERSION 3.5)
project(MP3Encoder)

if (WIN32)
    include_directories(lame-3.100/include/ pthread/include)
    link_directories(lame-3.100/lib pthread/lib/x86)
endif (WIN32)
if (UNIX)
    include_directories(lame-3.100/include/)
    link_directories(lame-3.100/lib)
endif (UNIX)

add_executable(MP3Encoder src/main.cpp src/encoder.cpp src/encoder.h src/parallel_encoder.cpp src/parallel_encoder.h)

if (WIN32)
    target_link_libraries(MP3Encoder libmp3lame-static.lib pthreadVC2.lib)
endif (WIN32)
if (UNIX)
    target_link_libraries(MP3Encoder libmp3lame.a -lpthread)
endif (UNIX)
