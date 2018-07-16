#include "Sphere.hpp"

glm::dvec3 Sphere::findNormal(const glm::dvec3& intersection, const Ray& ray) const
{
	return glm::normalize(intersection - center);
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

static double	closest_dist(double dist1, double dist2)
{
	if (dist1 < dist2)
	{
		if (dist1 > 0)
			return (dist1);
		if (dist2 > 0)
			return (dist2);
		return (INFINITY);
	}
	if (dist2 > 0)
		return (dist2);
	if (dist1 > 0)
		return (dist1);
	return (INFINITY);
}

double Sphere::Intersection(const Ray& ray) const
{
	glm::dvec3 temp = ray.origin - center;
	double dist1 = dot(ray.direction, temp);
	double dist2;
	double discrim = dist1 * dist1 - dot(temp, temp) + radius * radius;

	if (discrim < 0)
		return INFINITY;
	discrim = sqrt(discrim);
	dist2 = -dist1 - discrim;
	dist1 = -dist1 + discrim;
	return closest_dist(dist1, dist2);
}
