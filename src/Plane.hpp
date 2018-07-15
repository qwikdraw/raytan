#pragma once

#include "Raetan.hpp"
#include "IObject.hpp"
#include "Sampler.hpp"

class	Plane : public IObject
{
public:
	double		Intersection(const Ray& ray) const;
	RayResult	MakeRayResult(double distance, const Ray& ray) const;

	glm::dvec3 center;
	glm::dvec3 normal;
	double refractiveIndex;
	double diffuse;
	double reflect;
	double refract;

	// defaults to color if no loaded texture sample, else it uses
	// the sampler
	glm::dvec3 color;
	Sampler colorSampler;
};
