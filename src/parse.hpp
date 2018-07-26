#pragma once

#include <stdint.h>

#include "Raytan.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

Scene*	ParseSceneFile(const std::string sceneFile);
