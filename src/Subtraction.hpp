#pragma once

#include "Raetan.hpp"
#include "IObject.hpp"

class	Subtraction : public IObject
{
	IObject const* _positive;
	IObject const* _negative;

	bool    isFacing(double dist, IObject* obj, const Ray& ray) const;
	
	std::vector<std::tuple<double, IObject*, bool, bool>>
	getEdges(const std::vector<std::pair<double, IObject*>>& positiveEdges,
		  const std::vector<std::pair<double, IObject*>>& negativeEdges,
		  const Ray& ray) const;
	
	std::vector<std::pair<double, IObject*>> findDistances(const Ray& ray) const;
	glm::dvec3 findNormal(const glm::dvec3& intersection, const Ray& ray) const;
	glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;
public:
	Subtraction(IObject *positive, IObject *negative);
	RayResult MakeRayResult(double distance, const Ray& ray, IObject*) const;
};
