#pragma once

#include "Raetan.hpp"
#include "IObject.hpp"

class	Plane : public IObject
{
	glm::dvec3	_center;
	glm::dvec3	_normal;

public:

	Plane(glm::dvec3 center, glm::dvec3 normal);

	double		Intersection(const Ray& ray) const;
	RayResult	MakeRayResult(double distance, const Ray& ray) const;

};
