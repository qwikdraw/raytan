#include "Sphere.hpp"

Sphere::Sphere(glm::dvec3 center, double radius) :
_center(center),
_radius(radius)
{}

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
	glm::dvec3 temp = ray.origin - _center;
	double dist1 = dot(ray.direction, temp);
	double dist2;
	double discrim = dist1 * dist1 - dot(temp, temp) + _radius * _radius;

	if (discrim < 0)
		return glm::dvec3(INFINITY);
	discrim = sqrt(discrim);
	dist2 = -dist1 - discrim;
	dist1 = -dist1 + discrim;
	return closest_dist(dist1, dist2);
}

RayResult Sphere::MakeRayResult(double distance, const Ray& ray) const
{
	RayResult out;

	out.position = ray.origin + ray.direction * distance;
	out.normal = glm::normalize(out.position - _center);
	out.color = glm::dvec3(1, 0, 0);
	out.diffuse = 1;
	out.reflect = 0;
	out.refract = 0;
	out.refractiveIndex = 1;

	return out;
}
