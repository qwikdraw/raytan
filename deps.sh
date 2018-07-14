#!/bin/sh

brew install glfw --HEAD
brew install pkg-config glm sfml wget
export LD_LIBRARY_PATH=~/.brew/lib
mkdir -p lib
cd lib
mkdir -p lodepng
cd lodepng
wget https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.cpp
wget https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.h
