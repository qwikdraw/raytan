#include "Plane.hpp"

Plane::Plane(glm::dvec3 center, glm::dvec3 normal) :
_center(center),
_normal(normal)
{}

double Plane::Intersection(const Ray& ray) const
{
	double		denom;
	glm::dvec3	subtract;
	double		dist;

	denom = glm::dot(_normal, ray.direction);
	if (glm::abs(denom) < 0.0001)
		return INFINITY;
	subtract = ray.origin - _center;
	dist = glm::dot(subtract, ray.direction) / denom;
	return (dist > 0 ? dist : INFINITY);
}

RayResult Plane::MakeRayResult(double distance, const Ray& ray) const
{
	RayResult out;

	out.position = ray.origin + ray.direction * distance;
	if (glm::dot(ray.direction * -1, _normal) < 0)
		out.normal = _normal * -1;
	else
		out.normal = _normal;
	out.color = glm::dvec3(1, 0, 0);
	out.diffuse = 1;
	out.reflect = 0;
	out.refract = 0;
	out.refractiveIndex = 1;
	return out;
}
