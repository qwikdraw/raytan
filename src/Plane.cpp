#include "Plane.hpp"

SmallVector<double> Plane::findDistances(const Ray& ray) const
{
	double dist = glm::dot(ray.direction, direction);
	if (dist == 0)
		return SmallVector<double>();
	dist = glm::dot(-ray.origin, direction) / dist;

	SmallVector<double> out;
	out.push_back(dist);
	return out;
}

glm::dvec3 Plane::findNormal(const glm::dvec3& intersection) const
{
	return direction;
}


glm::dvec2 Plane::uvMap(const glm::dvec3& intersection, const glm::dvec3&) const
{
	glm::dvec2	out;

	out.x = intersection.x;
	out.y = intersection.z;
	return glm::mod(out, 1.0);
}
