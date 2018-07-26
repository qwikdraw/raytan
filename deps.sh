#!/bin/sh

brew install pkg-config glm qt wget
brew link --force qt
mkdir -p lib
cd lib
mkdir -p lodepng
cd lodepng
wget https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.cpp
wget https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.h
cd ..
mkdir -p json
cd json
wget https://github.com/nlohmann/json/releases/download/v3.1.2/json.hpp
