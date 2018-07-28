#include "Cylinder.hpp"

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

glm::dvec3		Cylinder::findNormal(const glm::dvec3& intersection) const
{

	glm::dvec3 tmp = intersection;

	double dist = glm::dot(tmp, direction);

	tmp = (direction * dist);

	return glm::normalize(intersection - tmp);
}

glm::dvec2		Cylinder::uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const
{
	glm::dvec2	out;

	double twoPiR = 2.0 * glm::pi<double>() * radius;
	double radiansAngle = glm::acos(glm::dot(glm::dvec3(1, 0, 0), normal));

	if (glm::dot(glm::dvec3(0, 0, 1), normal) < 0)
		radiansAngle = 2 * glm::pi<double>() - radiansAngle;

	out.x = radiansAngle / (2 * glm::pi<double>());
	out.y = glm::mod(intersection.y, twoPiR) / twoPiR;

	return out;
}

std::vector<double> Cylinder::findDistances(const Ray& ray) const
{
	// Get a
	// double dot = glm::dot(ray.direction, direction);
	// glm::dvec3 tmp = direction * dot;
	// glm::dvec3 a_temp = ray.direction - tmp;
	// double a = glm::dot(a_temp, a_temp);

	// Ray equation center + dir * dist

	double c = ray.origin.z * ray.origin.z + (0.001) - (ray.origin.x * ray.origin.x) - (ray.origin.y * ray.origin.y);
	double b = 2.0 * ((ray.origin.z * ray.direction.z) - (ray.origin.x * ray.direction.x) - (ray.origin.y * ray.direction.y));
	double a = (ray.direction.z * ray.direction.z) - (ray.direction.x * ray.direction.x) - (ray.direction.y * ray.direction.y);

	// Get b
	// glm::dvec3 delta_p = ray.origin;
	// tmp = direction * glm::dot(delta_p, direction);
	// glm::dvec3 b_temp = delta_p - tmp;
	// double b = 2 * glm::dot(a_temp, b_temp);

	// Get c
	// double c = glm::dot(b_temp, b_temp) - (radius * radius);

	// Return all distances
	glm::dvec2 root = solveQuadratic(a, b, c);
	
	std::vector<double> out;
	if (root.x)
		out.push_back(root.x);
	if (root.y && root.x != root.y)
		out.push_back(root.y);
	return out;
}
