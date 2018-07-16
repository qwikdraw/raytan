#pragma once

#include "Raetan.hpp"
#include "IObject.hpp"

class	Plane : public IObject
{
	std::vector<std::pair<double, IObject*>> findDistances(const Ray& ray) const;
	glm::dvec3 findNormal(const glm::dvec3& intersection, const Ray& ray) const;
	glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;
};
