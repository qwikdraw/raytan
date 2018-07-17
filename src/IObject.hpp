#pragma once

#include "Raytan.hpp"
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
	friend class Subtraction;
protected:
	virtual std::vector<std::pair<double, IObject*>> findDistances(const Ray& ray) const = 0;
	virtual glm::dvec3 findNormal(const glm::dvec3& intersection, const Ray& ray) const = 0;
	virtual glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const = 0;
	
public:
	IObject() {}
	virtual ~IObject() {}
	virtual std::pair<double, IObject*> Intersection(const Ray& ray) const;
	virtual RayResult MakeRayResult(double distance, const Ray& ray, IObject*) const;

	glm::dvec3 center;
	glm::dvec3 direction;

	double diffuse;
	double reflect;
	double refract;
	double refractiveIndex;
	Sampler *materialSampler;

	glm::dvec3 color;
	Sampler *colorSampler;

	Sampler *normalSampler;
};
