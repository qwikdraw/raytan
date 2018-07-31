#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"

// defines an Ellipse define by 3 'constants' (the squared of the respective radii, x-axis, y-axis, and z-axis)
class	Ellipse : public IObject
{
	glm::dvec2	solveQuadratic(double a, double b, double c) const;
	SmallVector<double> findDistances(const Ray& ray) const;
	glm::dvec3	findNormal(const glm::dvec3& intersection) const;
	glm::dvec2	uvMap(const glm::dvec3& intersection, const glm::dvec3& normal) const;

public:
	glm::dvec3	constant;
};
