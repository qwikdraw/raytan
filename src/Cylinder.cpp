#include "Cylinder.hpp"

// Cylinder::Cylinder(glm::dvec3 center, double radius, glm::dvec3 vector) :
// _center(center),
// _radius(radius),
// _vector(vector)
// {}


glm::dvec2		Cylinder::solveQuadratic(double a, double b, double c) const
{
	glm::dvec2	root;

	double determinant = (b * b) - (4 * a * c);
	if (determinant > 0)
	{
		double sqroot = glm::sqrt(determinant);
		root.x = (-b + sqroot) / (2 * a);
		root.y = (-b - sqroot) / (2 * a);
	}
	else if (determinant == 0)
	{
		root.x = -b / (2 * a);
		root.y = root.x;
	}
	else
	{
		root.x = 0;
		root.y = 0;
	}
	return (root);
}

glm::dvec3		Cylinder::normal(glm::dvec3 intersection, const Ray& ray) const
{

	glm::dvec3 normal = intersection - center;

	double dist = glm::dot(normal, vector);

	normal = center + (vector * dist);

	return (glm::normalize(intersection - normal));
}

double 			Cylinder::Intersection(const Ray& ray) const
{
	// Get a
	double dot = glm::dot(ray.direction, vector);
	glm::dvec3 tmp = vector * dot;
	glm::dvec3 a_temp = ray.direction - tmp;
	double a = glm::dot(a_temp, a_temp);

	// Get b
	glm::dvec3 delta_p = ray.origin - center;
	tmp = vector * glm::dot(delta_p, vector);
	glm::dvec3 b_temp = delta_p - tmp;
	double b = 2 * glm::dot(a_temp, b_temp);

	// Get c
	double c = glm::dot(b_temp, b_temp) - (radius * radius);

	// Calculate the distance
	glm::dvec2 root = solveQuadratic(a, b, c);
	double distance = 0;
	if (root.x > 0.0001 && root.x < root.y)
		distance = root.x;
	if (root.y > 0.0001 && root.y < root.x)
		distance = root.y;

	return (distance != 0 ? distance : INFINITY);
}

RayResult Cylinder::MakeRayResult(double distance, const Ray& ray) const
{
	RayResult out;

	out.position = ray.origin + (ray.direction * distance);
	out.normal = normal(out.position, ray);
	out.color = color;
	out.diffuse = diffuse;
	out.reflect = reflect;
	out.refract = refract;
	out.refractiveIndex = refractiveIndex;

	return out;
}
