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

glm::dvec3		Cone::findNormal(const glm::dvec3& intersection, const Ray& ray) const
{
	double dist = glm::length(intersection - center) / glm::cos(angle);
	glm::dvec3 tmpVector = intersection - center;

	if (glm::dot(tmpVector, vector) < 0)
		dist = -dist;
	tmpVector = center + (vector * dist);
	tmpVector = glm::normalize(intersection - tmpVector);
	return tmpVector;
}

glm::dvec2		Cone::uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const
{
	return glm::dvec2(0.5, 0.5); // Will  update later
}

std::vector<std::pair<double, IObject*>> Cone::findDistances(const Ray& ray) const
{
	// Get a
	double tmpA = glm::dot(ray.direction, vector);
	double cos2 = angle * angle;
	double a = tmpA * tmpA - cos2;

	// Get b
	glm::dvec3 tmpVector = ray.origin - center;
	double tmpB = glm::dot(tmpVector, vector);
	double b = 2 * (tmpA * tmpB - (glm::dot(ray.direction, tmpVector) * cos2));

	// Get c
	double c = tmpB * tmpB - (glm::dot(tmpVector, tmpVector) * cos2);

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

RayResult Cone::MakeRayResult(double distance, const Ray& ray) const
{
	RayResult out;

	out.position = ray.origin + (ray.direction * distance);
	out.normal = findNormal(out.position, ray);
	out.color = color;
	out.diffuse = diffuse;
	out.reflect = reflect;
	out.refract = refract;
	out.refractiveIndex = refractiveIndex;

	return out;
}
