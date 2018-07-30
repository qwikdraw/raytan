#include "Noise2D.hpp"

glm::dvec4 Noise2D::checkerboard(double x, double y)
{
	if (x < 0.5)
	{
		if (y < 0.5)
			return glm::dvec4(0.001, 0.001, 0.001, 0.0);
		return glm::dvec4(0.9, 0.9, 0.9, 0.1);
	}
	if (y < 0.5)
		return glm::dvec4(0.9, 0.9, 0.9, 0.1);
	return glm::dvec4(0.001, 0.001, 0.001, 0.0);
}

glm::dvec4 Noise2D::sineWave(double x, double y)
{
	(void)x;
	(void)y;
	return glm::dvec4(0, 0, 0, 0);
}
