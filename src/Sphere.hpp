#pragma once

#include "Raetan.hpp"
#include "IObject.hpp"

class	Sphere : public IObject
{
	glm::dvec3 findNormal(const glm::dvec3& intersection, const Ray& ray) const;
	glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;
public:
	double Intersection(const Ray& ray) const;

	double radius;
};
