#include "Ellipse.hpp"

glm::dvec2		Ellipse::solveQuadratic(double a, double b, double c) const
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

glm::dvec3		Ellipse::findNormal(const glm::dvec3& intersection) const
{
	glm::dvec3	normal;

	// Original, unoptimized version
	// normal.x = 2.0 * intersection.x / ((constant.x / 2.0) * (constant.x / 2.0));
	// normal.y = 2.0 * intersection.y / ((constant.y / 2.0) * (constant.y / 2.0));
	// normal.z = 2.0 * intersection.z / ((constant.z / 2.0) * (constant.z / 2.0));

	// New, slightly cheaty-but-looks-okay optimized version
	normal.x = 2.0 * intersection.x / constant.x;
	normal.y = 2.0 * intersection.y / constant.y;
	normal.z = 2.0 * intersection.z / constant.z;

	return glm::normalize(normal);
}

glm::dvec2		Ellipse::uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const
{
	glm::dvec2 out;

	out.x = glm::atan(normal.x, normal.z) / (2 * 3.1415) + 0.5;
	out.y = normal.y / 2.0 + 0.5;

	return out;
}

SmallVector<double> Ellipse::findDistances(const Ray& ray) const
{
	// Old, algorithmically-correct, version
	// glm::dvec3 e2(constant.x * constant.x, constant.y * constant.y, constant.z * constant.z);
	// double bc = e2.y * e2.z;
	// double ac = e2.x * e2.z;
	// double ab = e2.x * e2.y;

	// double a =	(ray.direction.x * ray.direction.x * bc) +
	// 			(ray.direction.y * ray.direction.y * ac) +
	// 			(ray.direction.z * ray.direction.z * ab);

	// double b =	((ray.origin.x * ray.direction.x * bc) +
	// 			(ray.origin.y * ray.direction.y * ac) +
	// 			(ray.origin.z * ray.direction.z * ab)) * 2.0;

	// double c =	(ray.origin.x * ray.origin.x * bc) +
	// 			(ray.origin.y * ray.origin.y * ac) +
	// 			(ray.origin.z * ray.origin.z * ab) - (bc * ac * ab);

	// New, Still works, slightly optimized version that skips the initial double-creating step
	double bc = constant.y * constant.z;
	double ac = constant.x * constant.z;
	double ab = constant.x * constant.y;

	double a =	(ray.direction.x * ray.direction.x * bc) +
				(ray.direction.y * ray.direction.y * ac) +
				(ray.direction.z * ray.direction.z * ab);

	double b =	((ray.origin.x * ray.direction.x * bc) +
				(ray.origin.y * ray.direction.y * ac) +
				(ray.origin.z * ray.direction.z * ab)) * 2.0;

	double c =	(ray.origin.x * ray.origin.x * bc) +
				(ray.origin.y * ray.origin.y * ac) +
				(ray.origin.z * ray.origin.z * ab) - (bc * ac * ab);

	// Return all distances
	glm::dvec2 root = solveQuadratic(a, b, c);
	
	SmallVector<double> out;
	if (root.x)
		out.push_back(root.x);
	if (root.y && root.x != root.y)
		out.push_back(root.y);
	return out;
}
