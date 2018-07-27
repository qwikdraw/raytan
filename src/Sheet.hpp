#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"

// defines a plane centered at the origin and facing the direction of constexpr IObject::direction
class	Sheet : public IObject
{
		std::vector<double> findDistances(const Ray& ray) const;
		glm::dvec3 findNormal(const glm::dvec3& intersection) const;
		glm::dvec2 uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;

	public:
		glm::dvec2	size;
};
