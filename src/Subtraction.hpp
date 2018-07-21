#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"

struct Edge
{
	Intersect inter;
	bool forwardFacing;
};

// non primitive object
class	Subtraction : public IObject
{
	IObject const* _positive;
	IObject const* _negative;

	bool isFacing(const Intersect&, const Ray&) const;
	std::vector<Edge> generateEdges(const std::vector<Intersect>&,
					const std::vector<Intersect>&,
					const Ray&) const;
	std::vector<Intersect> getIntersectionsFrom(const IObject*, const Ray&) const;
	std::vector<Intersect> findIntersections(const Ray&) const;
	
	// will have dummy implementation
	std::vector<double> findDistances(const Ray&) const;
public:
	Subtraction(IObject *positive, IObject *negative);

	bool IsPrimitive(void) const;
	Intersect Intersection(const Ray&) const;
};
