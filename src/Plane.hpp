#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"

// defines a plane centered at the origin and facing the direction of constexpr IObject::direction
class	Plane : public IObject
{
	std::vector<std::pair<double, IObject*>> findDistances(const Ray& ray) const;
	glm::dvec3 findNormal(const glm::dvec3& intersection, const Ray& ray) const;
	glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;
};
