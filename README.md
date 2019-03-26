**MP3Encoder** is a simple C++ command line application that encodes a set of WAV files to MP3


# Features
* Converts all WAV files to MP3 in the provided folder
* Written in C++ by using LAME MP3 Encoder library and POSIX threads
* Each encoding process runs in a different thread to utilize multi-threading
* Compilable and runnable on Windows and Linux

# Compilation
cmake minimum required version is 3.5 in order to compile.

## Linux
Change your current directory to the project folder and run the following commands;
* `cmake .`
* `make`

## Windows
* Open VS Native Tools Command Prompt, change your current directory to the project folder and run `cmake . ` command. This will generate a Visual Studio solution,
* Open the solution in Visual Studio and build,
* Copy pthreadVC2.dll (in `pthread/dll` folder) into the generated build folder.


# Usage
`MP3Encoder <WAVCollectionFolder>`