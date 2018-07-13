#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define GLFW_INCLUDE_GLCOREARB

#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

struct	Ray
{
	glm::dvec3 origin;
	glm::dvec3 direction;
	double refractiveIndex;
};

struct	RawColor
{
	glm::dvec3 color;
	double depth;
};
