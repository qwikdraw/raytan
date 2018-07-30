#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"

struct Edge
{
	Intersect inter;
	bool positive;
	bool forwardFacing;
};

// non primitive object
class	Subtraction : public IObject
{
	IObject const* _positive;
	IObject const* _negative;

	bool isFacing(const Intersect&, const Ray&) const;
	inline std::vector<Edge> generateEdges(const SmallVector<Intersect>&,
					const SmallVector<Intersect>&,
					const Ray&) const;
	SmallVector<Intersect> getIntersectionsFrom(const IObject*, const Ray&) const;
	SmallVector<Intersect> findIntersections(const Ray&) const;
	
	// will have dummy implementation
	SmallVector<double> findDistances(const Ray&) const;
public:
	Subtraction(IObject *positive, IObject *negative);
	~Subtraction(void);

	bool IsPrimitive(void) const;
	Intersect Intersection(const Ray&) const;
};
