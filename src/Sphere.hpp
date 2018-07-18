#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"


//defines a sphere centered at the origin with a radius.
class	Sphere : public IObject
{
	std::vector<std::pair<double, IObject*>> findDistances(const Ray& ray) const;
	glm::dvec3 findNormal(const glm::dvec3& intersection, const Ray& ray) const;
	glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;

public:
	double radius;
};
