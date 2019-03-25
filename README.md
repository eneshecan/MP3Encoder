**MP3Encoder** is a simple C++ command line application that encodes a set of WAV files to MP3

## Features
* Converts all WAV files to MP3 in the provided folder
* Written in C++ by using LAME MP3 Encoder library and POSIX threads
* Each encoding process runs in a different thread to utilize multi-threading

## Compilation
At least cmake 3.5 should be installed in order to compile. Change your current directory to the project folder and run the following commands;

* `cmake .`
* `make`


## Usage
`MP3Encoder <WAVCollectionFolder>`