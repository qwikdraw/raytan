#include "IObject.hpp"

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
	if (ref->materialSampler || ref->colorSampler || ref->normalSampler)
		uv = ref->uvMap(out.position, out.normal);

	if (ref->materialSampler)
	{
		glm::dvec4 sample = ref->materialSampler->Color(uv.x, uv.y);
		out.diffuse = sample.r;
		out.reflect = sample.g;
		out.refract = sample.b;
		out.refractiveIndex = sample.a;
	}
	else
	{
		out.diffuse = ref->diffuse;
		out.reflect = ref->reflect;
		out.refract = ref->refract;
		out.refractiveIndex = ref->refractiveIndex;
	}

	if (ref->colorSampler)
	{
		glm::dvec4 sample = ref->colorSampler->Color(uv.x, uv.y);
		out.color = glm::vec3(sample);
	}
	else
	{
		out.color = ref->color;
	}

//	if (ref->normalSampler)
//	{
//		//stuff
//	}

	return out;
}
