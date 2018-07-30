#include "Sheet.hpp"

SmallVector<double> Sheet::findDistances(const Ray& ray) const
{
	double dist = glm::dot(ray.direction, direction);
	SmallVector<double> out;
	if (dist == 0)
		return out;
	dist = glm::dot(-ray.origin, direction) / dist;
	glm::dvec3 intersect = ray.origin + (ray.direction * dist);
	// out of x-bounds
	if (glm::abs(intersect.x) > (size.x / 2))
		return out;
	// out of z-bounds
	if (glm::abs(intersect.z) > (size.y / 2))
		return out;

	// return dist
	out.push_back(dist);
	return out;
}

glm::dvec3 Sheet::findNormal(const glm::dvec3& intersection) const
{
	return direction;
}


glm::dvec2 Sheet::uvMap(const glm::dvec3& intersection, const glm::dvec3&) const
{
	glm::dvec2	out;

	out.x = intersection.x;
	out.y = intersection.z;
	return glm::mod(out, 1.0);
}
