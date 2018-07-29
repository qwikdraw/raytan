#pragma once

#include <stdint.h>

#include "Raytan.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

struct RT
{
	Camera	camera;
	Scene	scene;	
};

RT*	ParseSceneFile(const std::string sceneFile);
