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
	virtual glm::dvec3 Intersection(const Ray& ray) const = 0;
	virtual RayResult MakeRayResult(const glm::dvec3& position, const Ray& ray) const = 0;
};
