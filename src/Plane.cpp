#include "Plane.hpp"

Plane::Plane(glm::dvec3 center, glm::dvec3 normal) :
_center(center),
_normal(normal)
{}

double Plane::Intersection(const Ray& ray) const
{
	double dist = glm::dot(ray.direction, _normal);
	if (dist == 0)
		return INFINITY;
	dist = glm::dot(_center - ray.origin, _normal) / dist;
	if (dist < 0)
		return INFINITY;
	return dist;
}

RayResult Plane::MakeRayResult(double distance, const Ray& ray) const
{
	RayResult out;

	out.position = ray.origin + ray.direction * distance;

	if (glm::dot(ray.direction, _normal) < 0)
		out.normal = _normal;
	else
		out.normal = _normal * -1.0;
	
	out.color = glm::dvec3(1, 1, 1);
	out.diffuse = 1;
	out.reflect = 0;
	out.refract = 0;
	out.refractiveIndex = 1;
	return out;
}
