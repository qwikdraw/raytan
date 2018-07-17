#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"
#include "Sampler.hpp"

class	Cube : public IObject
{
	glm::dvec2	solveQuadratic(double a, double b, double c) const;
	glm::dvec3	findNormal(const glm::dvec3& intersection, const Ray& ray) const;
	std::vector<std::pair<double, IObject*>> findDistances(const Ray& ray) const;
	glm::dvec2	uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;

public:
	// [0] is min, [1] is max
	glm::dvec3	boundary[2];
};
