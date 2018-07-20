#include "Sphere.hpp"

std::vector<double> Sphere::findDistances(const Ray& ray) const
{
	double dist1 = dot(ray.direction, ray.origin);
	double dist2;
	double discrim = dist1 * dist1 - dot(ray.origin, ray.origin) + radius * radius;

	if (discrim < 0)
		return std::vector<double>();
	discrim = sqrt(discrim);
	dist2 = -dist1 - discrim;
	dist1 = -dist1 + discrim;

	std::vector<double> out;

	out.push_back(dist1);
	out.push_back(dist2);
	return out;
}

glm::dvec3 Sphere::findNormal(const glm::dvec3& intersection) const
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
