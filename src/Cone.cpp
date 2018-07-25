#include "Cone.hpp"

glm::dvec2		Cone::solveQuadratic(double a, double b, double c) const
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

glm::dvec3		Cone::findNormal(const glm::dvec3& intersection) const
{
	double dist = glm::length(intersection) / glm::cos(glm::radians(angle));
	glm::dvec3 tmpVector = intersection;

	if (glm::dot(tmpVector, direction) < 0)
		dist = -dist;
	tmpVector = (direction * dist);

	return glm::normalize(intersection - tmpVector + glm::dvec3(0.0001));
}

glm::dvec2		Cone::uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const
{

	glm::dvec2	out;
	// double		radiusAtIntersection = glm::abs(glm::tan(glm::radians(angle)) * intersection.y);
	// double		twoPiR = 2.0 * glm::pi<double>() * radiusAtIntersection;

	glm::dvec3	newNormal = glm::normalize(glm::dvec3(normal.x, 0, normal.z));
	double radiansAngle = glm::acos(glm::dot(glm::dvec3(1, 0, 0), newNormal));

	if (glm::dot(glm::dvec3(0, 0, 1), newNormal) < 0)
		radiansAngle = 2 * glm::pi<double>() - radiansAngle;

	out.x = radiansAngle / (2 * glm::pi<double>());
	out.y = glm::mod(glm::abs(intersection.y), 1.0); //twoPiR) / twoPiR;

	return out;
}

std::vector<double> Cone::findDistances(const Ray& ray) const
{
	// Get a
	double tmpA = glm::dot(ray.direction, direction);
	double cos2 = glm::cos(glm::radians(angle));
	cos2 = cos2 * cos2;
	double a = tmpA * tmpA - cos2;

	// Get b
	glm::dvec3 tmpVector = ray.origin;
	double tmpB = glm::dot(tmpVector, direction);
	double b = 2 * (tmpA * tmpB - (glm::dot(ray.direction, tmpVector) * cos2));

	// Get c
	double c = tmpB * tmpB - (glm::dot(tmpVector, tmpVector) * cos2);

	// Return all distances
	glm::dvec2 root = solveQuadratic(a, b, c);

	std::vector<double> out;
	if (root.x)
		out.push_back(root.x);
	if (root.y && root.x != root.y)
		out.push_back(root.y);
	return out;
}
