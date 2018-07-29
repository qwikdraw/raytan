#pragma once

#include "Raytan.hpp"
#include "Sampler.hpp"
#include "SmallVector.hpp"

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
};

class IObject;

struct Intersect
{
	double distance; // distance along ray that intersection happened
	const IObject *hitRef; // pointer to the primitive object
	Transform transform; // relative -> world
	bool positive;
};

class   IObject
{
	friend class Subtraction;
	friend class Addition;
protected:

	// method is not implemented for primitives
	virtual	SmallVector<Intersect> findIntersections(const Ray& ray) const;

	virtual SmallVector<double> findDistances(const Ray& ray) const = 0;

	// methods are not implemented for non primitives
	virtual glm::dvec3 findNormal(const glm::dvec3& intersection) const;
	virtual glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;
	
public:
	IObject() {}
	virtual ~IObject() {}

	virtual bool IsPrimitive(void) const;
	
	virtual Intersect Intersection(const Ray&) const;
	virtual RayResult MakeRayResult(const Intersect&, const Ray&) const;

	static glm::dvec3 TransformPoint(const glm::dvec3& p, const Transform&);
	static glm::dvec3 TransformVector(const glm::dvec3& v, const Transform&);
	static glm::dvec3 InverseTransformPoint(const glm::dvec3& p, const Transform&);
	static glm::dvec3 InverseTransformVector(const glm::dvec3& v, const Transform&);
	static Transform CompoundTransform(const Transform&, const Transform&);
	
	// used to define the direction of things like planes, cones ect
	static constexpr glm::dvec3 direction = {0, 1, 0};

	Transform transform;	
	Material material;
};
