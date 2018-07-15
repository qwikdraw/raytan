#pragma once

#include "Raetan.hpp"
#include "IObject.hpp"
#include "Sampler.hpp"

class	Cylinder : public IObject
{
	glm::dvec3	normal(glm::dvec3 intersection, const Ray& ray) const;
	glm::dvec2	solveQuadratic(double a, double b, double c) const;

public:
	glm::dvec3		center;
	double			radius;
	glm::dvec3		vector;
	glm::dvec3		color;
	Sampler 		colorSampler;
	double			refractiveIndex;
	double			diffuse;
	double			reflect;
	double			refract;

	// Cylinder(glm::dvec3 center, double radius, glm::dvec3 vector);

	double Intersection(const Ray& ray) const;
	RayResult MakeRayResult(double distance, const Ray& ray) const;
};
