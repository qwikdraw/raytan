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

	std::function<glm::dvec4(double, double)> _formula2D;
	bool _hasFormula2D;
	std::function<glm::dvec4(double, double, double)> _formula3D;
	bool _hasFormula3D;

public:

	Sampler(const std::string& filepath);
	Sampler(const std::function<glm::dvec4(double, double)>& formula2D);
	Sampler(const std::function<glm::dvec4(double, double, double)>& forumla3D);

	// returns a color from the texture based on normalized position
	// color is based on GL_NEAREST and GL_CLAMP_TO_EDGE
	glm::dvec4	Color(double x, double y) const;
	glm::dvec4	Color(double x, double y, double z) const;
};
