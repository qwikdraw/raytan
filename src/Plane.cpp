#include "Plane.hpp"

double Plane::Intersection(const Ray& ray) const
{
	double dist = glm::dot(ray.direction, normal);
	if (dist == 0)
		return INFINITY;
	dist = glm::dot(center - ray.origin, normal) / dist;
	if (dist < 0)
		return INFINITY;
	return dist;
}

static void	uv_map(double& x, double& y, const glm::dvec3& position, const glm::dvec3& center)
{
	// ill do later
	x = 0.5;
	y = 0.5;
}

RayResult Plane::MakeRayResult(double distance, const Ray& ray) const
{
	RayResult out;

	out.position = ray.origin + ray.direction * distance;
	if (glm::dot(ray.direction, normal) < 0)
		out.normal = normal;
	else
		out.normal = normal * -1.0;

	if (colorSampler.Empty())
		out.color = color;
	else
	{
		double x, y;
		uv_map(x, y, out.position, center);
		out.color = colorSampler.Color(x, y);
	}
	
	out.diffuse = diffuse;
	out.reflect = reflect;
	out.refract = refract;
	out.refractiveIndex = refractiveIndex;
	return out;
}
