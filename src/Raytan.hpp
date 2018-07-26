#pragma once

#define GLM_ENABLE_EXPERIMENTAL

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

#define IS_INFIN(a) (a.x == INFINITY)

struct	Ray
{
	glm::dvec3 origin;
	glm::dvec3 direction;
};

struct	RawColor
{
	glm::dvec3 color;
	double depth;
};

struct	Light
{
	glm::dvec3 position;
	glm::dvec3 color;
};
