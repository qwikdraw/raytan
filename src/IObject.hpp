#pragma once

#include "Raetan.hpp"
#include "Sampler.hpp"

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
protected:
	virtual glm::dvec3 findNormal(const glm::dvec3& intersection, const Ray& ray) const = 0;
	virtual glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const = 0;
	
public:
	IObject() {}
	virtual ~IObject() {}
	virtual double Intersection(const Ray& ray) const = 0;
	RayResult MakeRayResult(double distance, const Ray& ray) const;

	glm::dvec3 center;
	
	double diffuse;
	double reflect;
	double refract;
	double refractiveIndex;
	Sampler *materialSampler;

	glm::dvec3 color;
	Sampler *colorSampler;

	Sampler *normalSampler;
};
