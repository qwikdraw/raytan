#include "IObject.hpp"

constexpr glm::dvec3 IObject::direction;

std::pair<double, IObject*>	IObject::Intersection(const Ray& ray) const
{
	auto dists = findDistances(ray);	
	std::pair<double, IObject*> bestPair(INFINITY, nullptr);
	
	for (auto& pair : dists)
	{
		if (pair.first < bestPair.first && pair.first > 0)
			bestPair = pair;
	}
	return bestPair;
}

RayResult	IObject::MakeRayResult(double distance, const Ray& ray, IObject* ref) const
{
	RayResult out;

	out.position = ray.origin + ray.direction * distance;
	out.normal = ref->findNormal(out.position, ray);

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
