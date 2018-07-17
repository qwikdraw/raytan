#include "Cube.hpp"

glm::dvec2		Cube::solveQuadratic(double a, double b, double c) const
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

glm::dvec3		Cube::findNormal(const glm::dvec3& intersection, const Ray& ray) const
{
	// Getting the first vector from center to intersection point
	glm::dvec3 pVec = intersection - center;

	// Grab the 'divisor' vectors
	glm::dvec3 divisor = glm::abs(boundary[0] - boundary[1]) / 2.0;

	// Calculate the normal vector
	glm::dvec3 normal;
	normal.x = (int)((pVec.x / divisor.x) * 1.00001);
	normal.y = (int)((pVec.y / divisor.y) * 1.00001);
	normal.z = (int)((pVec.z / divisor.z) * 1.00001);

	return glm::normalize(normal);
}

glm::dvec2		Cube::uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const
{
	return glm::dvec2(0.5, 0.5); // Will  update later
}

std::vector<std::pair<double, IObject*>> Cube::findDistances(const Ray& ray) const
{
	// Initializing inverse direction ray
	glm::dvec3 inverse_direction = 1.0 / ray.direction;

	// Calculating the min and max distance of points hit on cube
	double x1 = (center.x + boundary[0].x - ray.origin.x) * inverse_direction.x;
	double x2 = (center.x + boundary[1].x - ray.origin.x) * inverse_direction.x;
	double y1 = (center.y + boundary[0].y - ray.origin.y) * inverse_direction.y;
	double y2 = (center.y + boundary[1].y - ray.origin.y) * inverse_direction.y;
	double z1 = (center.z + boundary[0].z - ray.origin.z) * inverse_direction.z;
	double z2 = (center.z + boundary[1].z - ray.origin.z) * inverse_direction.z;

	double minDist = glm::max(glm::max(glm::min(x1, x2), glm::min(y1, y2)), glm::min(z1, z2));
	double maxDist = glm::min(glm::min(glm::max(x1, x2), glm::max(y1, y2)), glm::max(z1, z2));

	std::vector<std::pair<double, IObject*>>	out;

	if (minDist > maxDist) // Ray doesn't intersect object
		return out;
	out.push_back(std::pair<double, IObject*>(minDist, (IObject*)this));
	if (minDist == maxDist)
		return out;
	out.push_back(std::pair<double, IObject*>(maxDist, (IObject*)this));
	return out;
}

RayResult Cube::MakeRayResult(double distance, const Ray& ray) const
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
