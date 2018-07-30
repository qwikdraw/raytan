#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"
#include "Sampler.hpp"

// defines an axis alligned cuboid with size denoting the length of each side
class	Cube : public IObject
{
	glm::dvec2	solveQuadratic(double a, double b, double c) const;
	glm::dvec3	findNormal(const glm::dvec3& intersection) const;
	SmallVector<double> findDistances(const Ray& ray) const;
	glm::dvec2	uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;

public:
	glm::dvec3	size;
};
