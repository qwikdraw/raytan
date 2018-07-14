#pragma once

#include "Raetan.hpp"

struct  RayResult
{
	glm::dvec3 position;
	glm::dvec3 normal;
	glm::dvec3 color;
	double diffuse;
	double reflect;
	double refract;
	double refractiveIndex;
};

class   IObject
{
public:
	IObject() {}
	virtual ~IObject() {}
	virtual double Intersection(const Ray& ray) const = 0;
	virtual RayResult MakeRayResult(double distance, const Ray& ray) const = 0;
};
