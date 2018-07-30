#include "Noise2D.hpp"

glm::dvec4 Noise2D::checkerboard(double x, double y)
{
	if (x < 0.5)
	{
		if (y < 0.5)
			return glm::dvec4(0.0, 0.0, 0.0, 0.0);
		return glm::dvec4(0.9, 0.9, 0.9, 0.0);
	}
	if (y < 0.5)
		return glm::dvec4(0.9, 0.9, 0.9, 0.0);
	return glm::dvec4(0.0, 0.0, 0.0, 0.0);
}
