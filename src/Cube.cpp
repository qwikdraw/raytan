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
	// glm::dvec3 absI = glm::abs(intersection);
	// std::cout << absI.x << std::endl;

	// if (absI.x > absI.y && absI.x > absI.z && intersection.x < 0)
	// 	return glm::dvec3(-1, 0, 0);
	// if (absI.x > absI.y && absI.x > absI.z && intersection.x > 0)
	// 	return glm::dvec3(1, 0, 0);
	// if (absI.y > absI.x && absI.y > absI.z && intersection.y < 0)
	// 	return glm::dvec3(0, -1, 0);
	// if (absI.y > absI.x && absI.y > absI.z && intersection.y > 0)
	// 	return glm::dvec3(0, 1, 0);
	// if (intersection.z < 0)
	// 	return glm::dvec3(0, 0, -1);
	// return glm::dvec3(0, 0, 1);



	
	// Getting the first vector from center to intersection point
	glm::dvec3 pVec = intersection;

	// Grab the 'divisor' vectors
	glm::dvec3 divisor = glm::abs(size) / 2.0;

	// Calculate the normal vector
	glm::dvec3 normal =  glm::round(pVec);
	normal.x = (int)(pVec.x / divisor.x * 1.0001);
	normal.y = (int)(pVec.y / divisor.y * 1.0001);
	normal.z = (int)(pVec.z / divisor.z * 1.0001);

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
	double x1 = (-size.x / 2.0 - ray.origin.x) * inverse_direction.x;
	double x2 = (size.x / 2.0 - ray.origin.x) * inverse_direction.x;
	double y1 = (-size.y / 2.0 - ray.origin.y) * inverse_direction.y;
	double y2 = (size.y / 2.0 - ray.origin.y) * inverse_direction.y;
	double z1 = (-size.z / 2.0 - ray.origin.z) * inverse_direction.z;
	double z2 = (size.z / 2.0 - ray.origin.z) * inverse_direction.z;

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
