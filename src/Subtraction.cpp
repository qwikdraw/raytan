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
		      const Ray& rayP,
		      const Ray& rayN) const
{
	std::vector<std::tuple<double, IObject*, bool, bool>> out;
	std::tuple<double, IObject*, bool, bool> genericEdge;
	
	for (auto& edge : positiveEdges)
	{
		std::get<0>(genericEdge) = std::get<0>(edge);
		std::get<1>(genericEdge) = std::get<1>(edge);
		std::get<2>(genericEdge) = true;
		std::get<3>(genericEdge) = isFacing(std::get<0>(edge), std::get<1>(edge), rayP);

		out.push_back(genericEdge);
	}
	for (auto& edge : negativeEdges)
	{
		std::get<0>(genericEdge) = std::get<0>(edge);
		std::get<1>(genericEdge) = std::get<1>(edge);
		std::get<2>(genericEdge) = false;
		std::get<3>(genericEdge) = isFacing(std::get<0>(edge), std::get<1>(edge), rayN);
		
		out.push_back(genericEdge);
	}
	std::sort(out.begin(), out.end());
	
	return out;
}

std::vector<std::pair<double, IObject*>> Subtraction::findDistances(const Ray& ray) const
{
	Ray rayP = _positive->rayTransform(ray);
	ray rayN = _negative->rayTransform(ray);
	auto p = _positive->findDistances(rayP);
	auto n = _negative->findDistances(rayN);

	// tuple: dist, IObject*, is_positive?, is_entering?
	std::vector<std::tuple<double, IObject*, bool, bool>> edges = getEdges(p, n, rayP, rayN);
	
	bool insideP = false;
	bool insideN = false;

	// checking if ray starts off inside an object
	bool toggle1 = false;
	bool toggle2 = false;
	for (auto& edge : edges)
	{
		if (std::get<2>(edge) && !toggle1)
		{
			insideP = !std::get<3>(edge);
			toggle1 = true;
		}		
		if (!std::get<2>(edge) && !toggle2)
		{
			insideN = !std::get<3>(edge);
			toggle2 = true;
		}
		if (toggle1 && toggle2)
			break;
	}

	std::vector<std::pair<double, IObject*>> out;
	for (auto& edge : edges)
	{
		int event = insideP + 2 * insideN + 4 * std::get<2>(edge) + 8 * std::get<3>(edge);

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

			//printing error message
			for (auto& edge : edges)
			{
				std::cout << "edge: ";
				std::cout << std::get<0>(edge) << " "
					  << std::get<1>(edge) << " "
					  << std::get<2>(edge) << " "
					  << std::get<3>(edge) << std::endl;
			}
			
			std::cout << insideP << " " << insideN << " " << std::get<2>(edge) << " "
				  << std::get<3>(edge) << std::endl;
			assert(!"badly defined shape");
			break;
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

RayResult Subtraction::MakeRayResult(double distance, const Ray& ray, IObject* ref) const
{
	RayResult out;

	out.position = ray.origin + ray.direction * distance;
	out.normal = ref->findNormal(out.position, ray);

	if (ref == _negative)
		out.normal = -out.normal;

	glm::dvec2 uv;
	if (ref->material.materialSampler || ref->material.colorSampler || ref->material.normalSampler)
		uv = ref->uvMap(out.position, out.normal);
	
	if (ref->material.materialSampler)
	{
		glm::dvec4 sample = ref->material.materialSampler->Color(uv.x, uv.y);
		out.diffuse = sample.r;
		out.reflect = sample.g;
		out.refract = sample.b;
		out.refractiveIndex = sample.a;
	}
	else
	{
		out.diffuse = ref->material.diffuse;
		out.reflect = ref->material.reflect;
		out.refract = ref->material.refract;
		out.refractiveIndex = ref->material.refractiveIndex;
	}

	if (ref->material.colorSampler)
	{
		glm::dvec4 sample = ref->material.colorSampler->Color(uv.x, uv.y);
		out.color = glm::vec3(sample);
	}
	else
	{
		out.color = ref->material.color;
	}

//	if (ref->normalSampler)
//	{
//		//stuff
//	}

	return out;
}
