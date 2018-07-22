#include "Plane.hpp"

std::vector<std::pair<double, IObject*>> Plane::findDistances(const Ray& ray) const
{
	double dist = glm::dot(ray.direction, direction);
	if (dist == 0)
		return std::vector<std::pair<double, IObject*>>();
	dist = glm::dot(-ray.origin, direction) / dist;

	std::pair<double, IObject*> p;
	p.first = dist;
	p.second = (IObject*)this;
	std::vector<std::pair<double, IObject*>> out;
	out.push_back(p);
	
	return out;
}

glm::dvec3 Plane::findNormal(const glm::dvec3& intersection, const Ray& ray) const
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
