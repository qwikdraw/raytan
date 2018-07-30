#pragma once

#include "Raytan.hpp"
#include "IObject.hpp"
#include "Subtraction.hpp"

class	Addition : public IObject
{
	IObject const* _shape1;
	IObject const* _shape2;

	bool isFacing(const Intersect& i, const Ray& ray) const;
	inline std::vector<Edge> generateEdges(const SmallVector<Intersect>& s1,
					const SmallVector<Intersect>& s2,
					const Ray& ray) const;
	SmallVector<Intersect> getIntersectionsFrom(const IObject*, const Ray&) const;
	SmallVector<Intersect> findIntersections(const Ray&) const;

	// will have dummy implementation
	SmallVector<double> findDistances(const Ray&) const;
public:
	Addition(IObject* shape1, IObject* shape2);
	~Addition(void);

	bool IsPrimitive(void) const;
	Intersect Intersection(const Ray&) const;
};
