#include "IObject.hpp"

constexpr glm::dvec3 IObject::direction;

// a default implementation for primitive shapes. Compound shapes should implement their own
Intersect	IObject::Intersection(const Ray& ray) const
{
	std::vector<double> dists = findDistances(ray);
	double bestDist = INFINITY;

	for (auto d : dists)
	{
		if (d < bestDist && d > 0)
			bestDist = d;
	}

	Intersect out;
	out.distance = bestDist;
	out.hitRef = this;
	out.transform = transform;
	out.positive = true;
	
	return out;
}

RayResult	IObject::MakeRayResult(const Intersect& intersect, const Ray& ray) const
{
	RayResult out;

	// the world position
	out.position = ray.origin + ray.direction * intersect.distance;

	// because we are going from world -> relative
	glm::dvec3 relativePos = InverseTransformPoint(out.position, intersect.transform);

	// relative normal
	out.normal = intersect.hitRef->findNormal(relativePos);

	glm::dvec2 uv;
	if (intersect.hitRef->material.materialSampler ||
	    intersect.hitRef->material.colorSampler ||
	    intersect.hitRef->material.normalSampler)
	{
		uv = intersect.hitRef->uvMap(relativePos, out.normal);
	}

	// going from relative -> world
	out.normal = TransformVector(out.normal, intersect.transform);
	
	if (intersect.hitRef->material.materialSampler)
	{
		glm::dvec4 sample = intersect.hitRef->material.materialSampler->Color(uv.x, uv.y);
		out.diffuse = sample.r;
		out.reflect = sample.g;
		out.refract = sample.b;
		out.refractiveIndex = sample.a;
	}
	else
	{
		out.diffuse = intersect.hitRef->material.diffuse;
		out.reflect = intersect.hitRef->material.reflect;
		out.refract = intersect.hitRef->material.refract;
		out.refractiveIndex = intersect.hitRef->material.refractiveIndex;
	}

	if (intersect.hitRef->material.colorSampler)
	{
		glm::dvec4 sample = intersect.hitRef->material.colorSampler->Color(uv.x, uv.y);
		out.color = glm::vec3(sample);
	}
	else
	{
		out.color = intersect.hitRef->material.color;
	}

//	if (ref->normalSampler)
//	{
//		//stuff
//	}

	if (!intersect.positive)
		out.normal = -out.normal;
	
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
