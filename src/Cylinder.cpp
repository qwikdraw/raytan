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

glm::dvec3		Cylinder::findNormal(const glm::dvec3& intersection, const Ray& ray) const
{

	glm::dvec3 normal = intersection;

	double dist = glm::dot(normal, direction);

	normal = (direction * dist);

	return glm::normalize(intersection - normal);
}

glm::dvec2		Cylinder::uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const
{
	return glm::dvec2(0.5, 0.5); // Will  update later
}

std::vector<std::pair<double, IObject*>> Cylinder::findDistances(const Ray& ray) const
{
	// Get a
	double dot = glm::dot(ray.direction, direction);
	glm::dvec3 tmp = direction * dot;
	glm::dvec3 a_temp = ray.direction - tmp;
	double a = glm::dot(a_temp, a_temp);

	// Get b
	glm::dvec3 delta_p = ray.origin;
	tmp = direction * glm::dot(delta_p, direction);
	glm::dvec3 b_temp = delta_p - tmp;
	double b = 2 * glm::dot(a_temp, b_temp);

	// Get c
	double c = glm::dot(b_temp, b_temp) - (radius * radius);

	// Return all distances
	glm::dvec2 root = solveQuadratic(a, b, c);
	std::pair<double, IObject*> p;
	p.first = root.x;
	p.second = (IObject*)this;
	std::vector<std::pair<double, IObject*>> out;
	out.push_back(p);
	p.first = root.y;
	out.push_back(p);
	return out;
}
