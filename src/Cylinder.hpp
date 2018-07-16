#pragma once

#include "Raetan.hpp"
#include "IObject.hpp"
#include "Sampler.hpp"

class	Cylinder : public IObject
{
	glm::dvec2	solveQuadratic(double a, double b, double c) const;
	glm::dvec3	findNormal(const glm::dvec3& intersection, const Ray& ray) const;
	glm::dvec2	uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;

public:
	double			radius;
	glm::dvec3		vector;

	// Cylinder(glm::dvec3 center, double radius, glm::dvec3 vector);

	double Intersection(const Ray& ray) const;
	RayResult MakeRayResult(double distance, const Ray& ray) const;
};
