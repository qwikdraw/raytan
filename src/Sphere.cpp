#include "Sphere.hpp"

std::vector<std::pair<double, IObject*>> Sphere::findDistances(const Ray& ray) const
{
	double dist1 = dot(ray.direction, ray.origin);
	double dist2;
	double discrim = dist1 * dist1 - dot(ray.origin, ray.origin) + radius * radius;

	if (discrim < 0)
		return std::vector<std::pair<double, IObject*>>();
	discrim = sqrt(discrim);
	dist2 = -dist1 - discrim;
	dist1 = -dist1 + discrim;

	std::vector<std::pair<double, IObject*>> out;

	out.push_back(std::pair<double, IObject*>(dist2, (IObject*)this));
	out.push_back(std::pair<double, IObject*>(dist1, (IObject*)this));
	return out;
}

glm::dvec3 Sphere::findNormal(const glm::dvec3& intersection, const Ray& ray) const
{
	return glm::normalize(intersection);
}

glm::dvec2 Sphere::uvMap(const glm::dvec3&, const glm::dvec3& normal) const
{
	glm::dvec2 out;

	out.x = glm::acos(glm::dot(normal, glm::dvec3(1, 0, 0)));
	out.y = glm::acos(glm::dot(normal, glm::dvec3(0, 1, 0)));
	out.x = out.x / 3.1415;
	out.y = out.y / 3.1415;

	return out;
}
