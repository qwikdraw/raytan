#pragma once

#include "Raetan.hpp"
#include "IObject.hpp"

class	Sphere : public IObject
{
	glm::dvec3 _center;
	double _radius;
public:
	Sphere(glm::dvec3 center, double radius);

	glm::dvec3 Intersection(const Ray& ray) const;
	RayResult MakeRayResult(const glm::dvec3& position, const Ray& ray) const;
};
