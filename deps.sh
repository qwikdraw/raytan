#!/bin/sh

brew install pkg-config glm glfw sfml wget
mkdir -p lib
cd lib
mkdir -p lodepng
cd lodepng
wget https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.cpp
wget https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.h
