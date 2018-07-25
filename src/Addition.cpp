#include "Addition.hpp"

std::vector<Intersect>	Addition::getIntersectionsFrom(const IObject* o, const Ray& ray) const
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

bool	Addition::isFacing(const Intersect& i, const Ray& ray) const
{
	glm::dvec3 hitPoint = ray.origin + ray.direction * i.distance;
	hitPoint = InverseTransformPoint(hitPoint, i.transform);

	glm::dvec3 normal = i.hitRef->findNormal(hitPoint);
	normal = TransformVector(normal, i.transform);

	if (!i.positive)
		normal = -normal;

	if (glm::dot(normal, ray.direction) < 0)
		return true;
	return false;
}

std::vector<Edge> Addition::generateEdges(const std::vector<Intersect>& s1,
					  const std::vector<Intersect>& s2,
					  const Ray& ray) const
{
	std::vector<Edge> out;
	Edge edge;

	for (const Intersect& i : s1)
	{
		edge.inter = i;
		edge.forwardFacing = isFacing(i, ray);
		edge.inter.transform = CompoundTransform(edge.inter.transform, transform);
		edge.positive = true;
		out.push_back(edge);
	}
	for (const Intersect& i : s2)
	{
		edge.inter = i;
		edge.forwardFacing = isFacing(i, ray);
		edge.inter.transform = CompoundTransform(edge.inter.transform, transform);
		edge.positive = false;
		out.push_back(edge);
	}

	std::sort(out.begin(),
		  out.end(),
		  [](Edge& a, Edge& b){return a.inter.distance < b.inter.distance;});

	return out;
}

std::vector<Intersect>	Addition::findIntersections(const Ray& ray) const
{
	auto s1 = getIntersectionsFrom(_shape1, ray);
	auto s2 = getIntersectionsFrom(_shape2, ray);

	std::vector<Edge> edges = generateEdges(s1, s2, ray);

	bool inside1 = false;
	bool inside2 = false;

	bool toggle1 = false;
	bool toggle2 = false;
	for (auto& edge : edges)
	{
		if (edge.positive && !toggle1)
		{
			inside1 = !edge.forwardFacing;
			toggle1 = true;
		}
		if (!edge.positive && !toggle2)
		{
			inside2 = !edge.forwardFacing;
			toggle2 = true;
		}
		if (toggle1 && toggle2)
			break;
	}

	std::vector<Intersect> out;
	for (auto& edge : edges)
	{
		int event = inside1 + 2 * inside2 + 4 * edge.positive + 8 * edge.forwardFacing;

		switch(event)
		{
		case(0b0011):
			inside1 = false;
			break;
		case(0b0111):
			inside2 = false;
			break;
		case(0b1010):
			inside1 = true;
			break;
		case(0b0110):
			out.push_back(edge.inter);
			inside2 = false;
			break;
		case(0b0001):
			out.push_back(edge.inter);
			inside1 = false;
			break;
		case(0b1101):
			inside2 = true;
			break;
		case(0b1000):
			out.push_back(edge.inter);
			inside1 = true;
			break;
		case(0b1100):
			out.push_back(edge.inter);
			inside2 = true;
			break;
		default:
			;
		}
	}
	return out;
}

std::vector<double>	Addition::findDistances(const Ray&) const
{
	assert(!"non primitive implementation should not be called");
	return std::vector<double>();
}

Addition::Addition(IObject* shape1, IObject* shape2)
{
	_shape1 = shape1;
	_shape2 = shape2;
}

Addition::~Addition(void)
{
	delete _shape1;
	delete _shape2;
}

bool	Addition::IsPrimitive(void) const
{
	return false;
}

Intersect	Addition::Intersection(const Ray& ray) const
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
