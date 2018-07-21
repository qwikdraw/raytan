#include "Subtraction.hpp"

std::vector<double>	Subtraction::findDistances(const Ray&) const
{
	assert(!"non primitive implementation should not be called");
	return std::vector<double>();
}


std::vector<Intersect>	Subtraction::getIntersectionsFrom(const IObject *o, const Ray& ray) const
{
	Ray transformed;
	transformed.origin = InverseTransformPoint(ray.origin, o->transform);
	transformed.direction = InverseTransformVector(ray.direction, o->transform);

	if (!o->IsPrimitive())
		return o->findIntersections(transformed);

	auto dists = o->findDistances(transformed);
	std::vector<Intersect> out;
	for (double d : dists)
	{
		Intersect i = {d, o, o->transform, true};
		out.push_back(i);
	}
	return out;

}

bool Subtraction::isFacing(const Intersect& i, const Ray& ray) const
{
	glm::dvec3 hitPoint = ray.origin + ray.direction * i.distance;
	hitPoint = InverseTransformPoint(hitPoint, i.hitRef->transform);

	glm::dvec3 normal = i.hitRef->findNormal(hitPoint);
	normal = TransformVector(normal, i.hitRef->transform);

	if (!i.positive)
		normal = -normal;
	
	if (glm::dot(normal, ray.direction) < 0)
		return true;
	return false;	
}

std::vector<Edge> Subtraction::generateEdges(const std::vector<Intersect>& p,
					     const std::vector<Intersect>& n,
					     const Ray& ray) const
{
	std::vector<Edge> out;
	Edge edge;

	for (const Intersect& i : p)
	{
		edge.inter = i;
		edge.forwardFacing = isFacing(i, ray);
		out.push_back(edge);
	}
	for (const Intersect& i : n)
	{
		edge.inter = i;
		edge.forwardFacing = isFacing(i, ray);
		out.push_back(edge);
	}

	std::sort(out.begin(),
		  out.end(),
		  [](Edge& a, Edge& b){return a.inter.distance < b.inter.distance;});
	
	return out;
}

std::vector<Intersect>	Subtraction::findIntersections(const Ray& ray) const
{
	auto p = getIntersectionsFrom(_positive, ray);
	auto n = getIntersectionsFrom(_negative, ray);

	for (Intersect& i : p)
	{
		i.transform = CompoundTransform(i.transform, transform);
	}
	for (Intersect& i : n)
	{
		i.transform = CompoundTransform(i.transform, transform);
		i.positive = !i.positive;
	}

	std::vector<Edge> edges = generateEdges(p, n, ray);
	bool insideP = false;
	bool insideN = false;

	// checking if ray starts off inside an object
	bool toggle1 = false;
	bool toggle2 = false;
	for (auto& edge : edges)
	{
		if (edge.inter.positive && !toggle1)
		{
			insideP = !edge.forwardFacing;
			toggle1 = true;
		}
		if (!edge.inter.positive && !toggle2)
		{
			insideN = !edge.forwardFacing;
			toggle2 = true;
		}
		if (toggle1 && toggle2)
			break;
	}
	std::vector<Intersect> out;
	for (auto& edge : edges)
	{
		int event = insideP + 2 * insideN + 4 * edge.inter.positive + 8 * edge.forwardFacing;

		switch(event)
		{
		case(0b1001):
			out.push_back(edge.inter);
			insideN = true;
			break;
		case(0b1000):
			insideN = true;
			break;
		case(0b0011):
			out.push_back(edge.inter);
			insideN = false;
			break;
		case(0b0010):
			insideN = false;
			break;
		case(0b1110):
			insideP = true;
			break;
		case(0b1100):
			out.push_back(edge.inter);
			insideP = true;
			break;
		case(0b0111):
			insideP = false;
			break;
		case(0b0101):
			out.push_back(edge.inter);
			insideP = false;
			break;
		default:
			assert(!"badly defined shape");
		}
	}
	return out;	
}

Subtraction::Subtraction(IObject *positive, IObject *negative)
{
	_positive = positive;
	_negative = negative;
}

bool	Subtraction::IsPrimitive(void) const
{
	return false;	
}

Intersect	Subtraction::Intersection(const Ray& ray) const
{
	Ray transformed;
	transformed.origin = InverseTransformPoint(ray.origin, transform);
	transformed.direction = InverseTransformVector(ray.direction, transform);
	
	auto hits = findIntersections(transformed);
	Intersect best;
	best.distance = INFINITY;

	for (Intersect& hit : hits)
	{
		if (hit.distance < best.distance && hit.distance > 0)
			best = hit;
	}
	return best;
}
