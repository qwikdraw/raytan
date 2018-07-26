#pragma once

#include "Raytan.hpp"

class	Camera
{
	glm::dvec3 _position;
	glm::dmat4 _deviceToWorld;
public:
	Camera(glm::dvec3 pos, glm::dvec3 dir, glm::dvec3 up, double fov, double aspect);

	// x and y should be normalized device coordinates
	Ray	GetRay(double x, double y) const;
};
