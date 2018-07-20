#include "IObject.hpp"

constexpr glm::dvec3 IObject::direction;

Ray	IObject::rayTransform(const Ray& ray) const
{
	Ray out;

	out.origin = ray.origin - position;
	
	out.direction = glm::rotateX(ray.direction, glm::radians(-rotation.x));
	out.direction = glm::rotateY(out.direction, glm::radians(-rotation.y));
	out.direction = glm::rotateZ(out.direction, glm::radians(-rotation.z));

	out.origin = glm::rotateX(out.origin, glm::radians(-rotation.x));
	out.origin = glm::rotateY(out.origin, glm::radians(-rotation.y));
	out.origin = glm::rotateZ(out.origin, glm::radians(-rotation.z));

	return out;
}

std::pair<double, IObject*>	IObject::Intersection(const Ray& ray) const
{
	auto dists = findDistances(rayTransform(ray));	
	std::pair<double, IObject*> bestPair(INFINITY, nullptr);
	
	for (auto& pair : dists)
	{
		if (pair.first < bestPair.first && pair.first > 0)
			bestPair = pair;
	}
	return bestPair;
}

RayResult	IObject::MakeRayResult(const Intersect& intersect, const Ray& ray) const
{
	RayResult out;

	out.position = ray.origin + ray.direction * intersect.distance;

	glm::dvec3 relativePos = InverseTransformPoint(out.position, intersect.transform);

	out.normal = ref->findNormal(relativePos);

	glm::dvec2 uv;
	if (intersect.hitRef->material.materialSampler ||
	    intersect.hitRef->material.colorSampler ||
	    intersect.hitRef->material.normalSampler)
	{
		uv = intersect.hitRef->uvMap(relativePos, out.normal);
	}

	out.normal = InverseTransformVector(out.normal, intersect.transform);
	
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

glm::dvec3 IObject::TransformPoint(const glm::dvec3& p, const Transform& t)
{
	glm::dvec3 out;
	
	out = glm::rotateX(p, glm::radians(t.rotation.x));
	out = glm::rotateY(out, glm::radians(t.rotation.y));
	out = glm::rotateZ(out, glm::radians(t.rotation.z));

	out = out + t.position;
	
	return out;
}

glm::dvec3 IObject::TransformVector(const glm::dvec3& v, const Transform& t)
{
	glm::dvec3 out;

	out = glm::rotateX(v, glm::radians(t.rotation.x));
	out = glm::rotateY(out, glm::radians(t.rotation.y));
	out = glm::rotateZ(out, glm::radians(t.rotation.z));

	return out;
}

glm::dvec3 IObject::InverseTransformPoint(const glm::dvec3& p, const Transform& t)
{
	glm::dvec3 out;

	out = p - t.position;
	
	out = glm::rotateZ(out, glm::radians(-t.rotation.z));
	out = glm::rotateY(out, glm::radians(-t.rotation.y));
	out = glm::rotateX(out, glm::radians(-t.rotation.x));

	return out;
}

glm::dvec3 IObject::InverseTransformVector(const glm::dvec3& v, const Transform& t)
{
	glm::dvec3 out;

	out = glm::rotateZ(v, glm::radians(-t.rotation.z));
	out = glm::rotateY(out, glm::radians(-t.rotation.y));
	out = glm::rotateX(out, glm::radians(-t.rotation.x));

	return out;
}
