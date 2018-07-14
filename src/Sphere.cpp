#include "Sphere.hpp"

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

static void	uv_map(double& x, double& y, const glm::dvec3& normal)
{
	x = glm::acos(glm::dot(normal, glm::dvec3(1, 0, 0)));
	y = glm::acos(glm::dot(normal, glm::dvec3(0, 1, 0)));
	x = x / 3.1415;
	y = y / 3.1415;
}

RayResult Sphere::MakeRayResult(double distance, const Ray& ray) const
{
	RayResult out;

	out.position = ray.origin + ray.direction * distance;
	out.normal = glm::normalize(out.position - center);

	if (colorSampler.Empty())
		out.color = color;
	else
	{
		double x, y;
		uv_map(x, y, out.normal);
		out.color = colorSampler.Color(x, y);
	}
	out.diffuse = diffuse;
	out.reflect = reflect;
	out.refract = refract;
	out.refractiveIndex = refractiveIndex;

	return out;
}
