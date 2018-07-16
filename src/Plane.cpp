#include "Plane.hpp"

glm::dvec3 Plane::findNormal(const glm::dvec3& intersection, const Ray& ray) const
{
		return direction;
}


glm::dvec2 Plane::uvMap(const glm::dvec3&, const glm::dvec3&) const
{
	// ill implement later
	return glm::dvec2(0);
}

double Plane::Intersection(const Ray& ray) const
{
	double dist = glm::dot(ray.direction, direction);
	if (dist == 0)
		return INFINITY;
	dist = glm::dot(center - ray.origin, direction) / dist;
	if (dist < 0)
		return INFINITY;
	return dist;
}
