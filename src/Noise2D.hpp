#pragma once

#include "Raytan.hpp"

namespace Noise2D {

	glm::dvec4 checkerboard(double x, double y);
	glm::dvec4 sinWave(double x, double y);
	glm::dvec4 customComplicatedAlgorithm(double x, double y);
	glm::dvec4 perlinNoise(double x, double y);

}