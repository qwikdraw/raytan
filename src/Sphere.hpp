#pragma once

#include "Raetan.hpp"
#include "IObject.hpp"

class	Sphere : public IObject
{
	glm::dvec3 _center;
	double _radius;
public:
	Sphere(glm::dvec3 center, double radius);

	double Intersection(const Ray& ray) const;
	RayResult MakeRayResult(double distance, const Ray& ray) const;
};
