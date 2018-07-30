#pragma once

#include "lodepng.h"
#include "Raytan.hpp"

class	Sampler
{
	std::vector<unsigned char> _imageData;
	unsigned _width;
	unsigned _height;
	bool _hasEquation;
	std::function<glm::dvec4(double, double)> _equation;

public:

	Sampler(const std::string& filepath);
	Sampler(std::function<glm::dvec4(double, double)> f);

	// returns a color from the texture based on normalized position (x, y)
	// color is based on GL_NEAREST and GL_CLAMP_TO_EDGE
	glm::dvec4	Color(double x, double y) const;
};
