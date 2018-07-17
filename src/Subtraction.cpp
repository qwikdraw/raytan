#include "Subtraction.hpp"

Subtraction::Subtraction(IObject *positive, IObject *negative)
{
	_positive = positive;
	_negative = negative;
}

bool	Subtraction::isFacing(double dist, IObject* obj, const Ray& ray) const
{
	glm::dvec3 intersection = ray.origin + ray.direction * dist;
	if (glm::dot(ray.direction, obj->findNormal(intersection, ray)) < 0)
		return true;
	return false;
}

std::vector<std::tuple<double, IObject*, bool, bool>>
Subtraction::getEdges(const std::vector<std::pair<double, IObject*>>& positiveEdges,
		      const std::vector<std::pair<double, IObject*>>& negativeEdges,
		      const Ray& ray) const
{
	std::vector<std::tuple<double, IObject*, bool, bool>> out;
	std::tuple<double, IObject*, bool, bool> genericEdge;
	
	for (auto& edge : positiveEdges)
	{
		std::get<0>(genericEdge) = std::get<0>(edge);
		std::get<1>(genericEdge) = std::get<1>(edge);
		std::get<2>(genericEdge) = true;
		std::get<3>(genericEdge) = isFacing(std::get<0>(edge), std::get<1>(edge), ray);

		out.push_back(genericEdge);
	}
	for (auto& edge : negativeEdges)
	{
		std::get<0>(genericEdge) = std::get<0>(edge);
		std::get<1>(genericEdge) = std::get<1>(edge);
		std::get<2>(genericEdge) = false;
		std::get<3>(genericEdge) = isFacing(std::get<0>(edge), std::get<1>(edge), ray);
		
		out.push_back(genericEdge);
	}
	std::sort(out.begin(), out.end());
	
	return out;
}

std::vector<std::pair<double, IObject*>> Subtraction::findDistances(const Ray& ray) const
{
	auto p = _positive->findDistances(ray);
	auto n = _negative->findDistances(ray);

	// tuple: dist, IObject*, is_positive?, is_entering?
	std::vector<std::tuple<double, IObject*, bool, bool>> edges = getEdges(p, n, ray);
	
	bool insideP = false;
	bool insideN = false;

	// checking if ray starts off inside an object
	for (auto& edge : edges)
	{
		if (std::get<0>(edge) > 0)
			break;
		if (std::get<2>(edge))
			insideP = std::get<3>(edge);
		if (!std::get<2>(edge))
			insideN = std::get<3>(edge);
	}

	std::vector<std::pair<double, IObject*>> out;
	for (auto& edge : edges)
	{
		int event = insideP + 2 * insideN + 4 * std::get<2>(edge) + 8 * std::get<3>(edge);

		std::cout << insideP << " " << insideN << " " << std::get<2>(edge) << " "
			  << std::get<3>(edge) << std::endl;

		std::cout << "positive: ";
		for (auto& x : p)
		{
			std::cout << x.first << " ";
		}
		std::cout << std::endl << "negative: ";
		for (auto& x : n)
		{
			std::cout << x.first << " ";
		}
		std::cout << std::endl;

		std::cout << "info: " << std::endl;
		for (auto& x : edges)
		{
			std::cout << std::get<0>(x) << " " << std::get<2>(x) << " " << std::get<3>(x) << std::endl;
		}
		
		switch(event)
		{
		case(0b1001):
			out.push_back(std::pair<double, IObject*>(std::get<0>(edge), std::get<1>(edge)));
			insideN = true;
			break;
		case(0b1000):
			insideN = true;
			break;
		case(0b0011):
			out.push_back(std::pair<double, IObject*>(std::get<0>(edge), std::get<1>(edge)));
			insideN = false;
			break;
		case(0b0010):
			insideN = false;
			break;
		case(0b1110):
			insideP = true;			
			break;
		case(0b1100):
			out.push_back(std::pair<double, IObject*>(std::get<0>(edge), std::get<1>(edge)));
			insideP = true;
			break;
		case(0b0111):			
			insideP = false;
			break;
		case(0b0101):
			out.push_back(std::pair<double, IObject*>(std::get<0>(edge), std::get<1>(edge)));
			insideP = false;
			break;
		default:
			assert(!"shape badly defined");
		}
	}
	return out;	
}

glm::dvec3 Subtraction::findNormal(const glm::dvec3&, const Ray&) const
{
	assert(!"dummy function was called");
	return glm::dvec3(0, 0, 0);
}

glm::dvec2 Subtraction::uvMap(const glm::dvec3&, const glm::dvec3&) const
{
	assert(!"dummy function was called");
	return glm::dvec2(0, 0);	
}
