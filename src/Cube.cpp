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

glm::dvec3		Cube::findNormal(const glm::dvec3& intersection) const
{
	double	bias = 0.00001;
	double	distX = intersection.x / (size.x / 2.0);
	double	distY = intersection.y / (size.y / 2.0);
	double	distZ = intersection.z / (size.z / 2.0);

	// Return X-facing normal
	if (distX > 0 && distX > 1 - bias)
		return glm::dvec3(1, 0, 0);
	if (distX < 0 && distX < bias - 1)
		return glm::dvec3(-1, 0, 0);

	// Return Y-facing normal
	if (distY > 0 && distY > 1 - bias)
		return glm::dvec3(0, 1, 0);
	if (distY < 0 && distY < bias - 1)
		return glm::dvec3(0, -1, 0);

	// Return Z-facing normal
	if (distZ > 0 && distZ > 1 - bias)
		return glm::dvec3(0, 0, 1);
	if (distZ < 0 && distZ < bias - 1)
		return glm::dvec3(0, 0, -1);

	// Return a normal based on the intersection to center (0, 0, 0) itself
	return glm::normalize(intersection);
}

glm::dvec2		Cube::uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const
{
	double	distX = (intersection.x / (size.x)) + 0.5;
	double	distY = (intersection.y / (size.y)) + 0.5;
	double	distZ = (intersection.z / (size.z)) + 0.5;

	// X-facing normal, product of changing y and z
	if (normal.x == 1 || normal.x == -1)
		return glm::dvec2(distY, distZ);
	if (normal.y == 1 || normal.y == -1)
		return glm::dvec2(distX, distZ);
	if (normal.z == 1 || normal.z == -1)
		return glm::dvec2(distX, distY);
	return glm::dvec2(0.5, 0.5);
}

std::vector<double> Cube::findDistances(const Ray& ray) const
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

	std::vector<double>	out;

	if (minDist > maxDist) // Ray doesn't intersect object
		return out;
	out.push_back(minDist);
	if (minDist == maxDist)
		return out;
	out.push_back(maxDist);
	return out;
}
