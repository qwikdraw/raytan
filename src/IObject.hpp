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

struct	Material
{
	double diffuse;
	double reflect;
	double refract;
	double refractiveIndex;
	Sampler *materialSampler;
	glm::dvec3 color;
	Sampler *colorSampler;
	Sampler *normalSampler;
};

class   IObject
{
	friend class Subtraction;
protected:

	Ray	rayTransform(const Ray& ray) const;
	
	virtual std::vector<std::pair<double, IObject*>> findDistances(const Ray& ray) const = 0;
	virtual glm::dvec3 findNormal(const glm::dvec3& intersection, const Ray& ray) const = 0;
	virtual glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const = 0;
	
public:
	IObject() {}
	virtual ~IObject() {}
	virtual std::pair<double, IObject*> Intersection(const Ray& ray) const;
	virtual RayResult MakeRayResult(double distance, const Ray& ray, IObject*) const;

	// used to define the direction of things like planes, cones ect
	static constexpr glm::dvec3 direction = {0, 1, 0};

	// do not use these variables in shape definitions. The ray will be transformed before
	// it arrives at the concrete shape's functions
	glm::dvec3 position;
	glm::dvec3 rotation;
	//__
	
	Material material;
};
