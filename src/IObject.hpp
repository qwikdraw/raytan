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

struct Transform
{
	glm::dvec3 position;
	glm::dvec3 rotation;
}

struct Intersect
{
	double distance; // distance along ray that intersection happened
	IObject *hitRef; // pointer to the primitive object
	Transform transform; // relative -> world
}

class   IObject
{
	friend class Subtraction;
protected:

	virtual std::vector<double> findDistances(const Ray& ray) const = 0;
	virtual glm::dvec3 findNormal(const glm::dvec3& intersection) const = 0;
	virtual glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const = 0;
	
public:
	IObject() {}
	virtual ~IObject() {}
	virtual Intersect Intersection(const Ray&) const;
	virtual RayResult MakeRayResult(const Intersect&, const Ray&) const;

	static glm::dvec3 TransformPoint(const glm::dvec3& p, const Transform&);
	static glm::dvec3 TransformVector(const glm::dvec3& v, const Transform&);
	static glm::dvec3 InverseTransformPoint(const glm::dvec3& p, const Transform&);
	static glm::dvec3 InverseTransformVector(const glm::dvec3& v, const Transform&);
	
	// used to define the direction of things like planes, cones ect
	static constexpr glm::dvec3 direction = {0, 1, 0};

	// do not use these variables in shape definitions. The ray will be transformed before
	// it arrives at the concrete shape's functions
	glm::dvec3 position;
	glm::dvec3 rotation;
	//__
	
	Material material;
};
