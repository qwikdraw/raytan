#pragma once

#include "lodepng.h"
#include "Raetan.hpp"

class	Sampler
{
	std::vector<unsigned char> _imageData;
	unsigned _width;
	unsigned _height;
	bool _empty = true;

public:

	void		Load(const std::string& filepath);

	// returns a color from the texture based on normalized position (x, y)
	// color is based on GL_NEAREST and GL_CLAMP_TO_EDGE
	glm::dvec3	Color(double x, double y) const;
	bool		Empty(void) const;
};
