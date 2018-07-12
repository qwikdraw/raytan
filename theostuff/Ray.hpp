#pragma once

#include "Raetan.hpp"

struct	Ray
{
	glm::dvec3 origin;
	glm::dvec3 direction;
	double refractiveIndex;
};
