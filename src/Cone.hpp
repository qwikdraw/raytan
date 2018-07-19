#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"
#include "Sampler.hpp"

// defines a infinite cone centered at origin, with IObject::direction as its axis
class	Cone : public IObject
{
	glm::dvec2	solveQuadratic(double a, double b, double c) const;
	glm::dvec3	findNormal(const glm::dvec3& intersection, const Ray& ray) const;
	std::vector<std::pair<double, IObject*>> findDistances(const Ray& ray) const;
	glm::dvec2	uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;

public:
	double			angle;
};
