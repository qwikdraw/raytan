#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"
#include "Sampler.hpp"

// defines a infinite cylinder centered at origin with IObject::direction as its axis
class	Cylinder : public IObject
{
	glm::dvec2	solveQuadratic(double a, double b, double c) const;
	std::vector<double> findDistances(const Ray& ray) const;
	glm::dvec3	findNormal(const glm::dvec3& intersection) const;
	glm::dvec2	uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;

public:
	double			radius;
};
