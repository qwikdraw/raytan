#include "IObject.hpp"

constexpr glm::dvec3 IObject::direction;

std::vector<Intersect>	IObject::findIntersections(const Ray&) const
{
	assert(!"default implementation should not be called");
	return std::vector<Intersect>();
}

glm::dvec3	IObject::findNormal(const glm::dvec3&) const
{
	assert(!"default implementation should not be called");
	return glm::dvec3();
}

glm::dvec2	IObject::uvMap(const glm::dvec3&, const glm::dvec3&) const
{
	assert(!"default implementation should not be called");
	return glm::dvec3();
}

bool	IObject::IsPrimitive(void) const
{
	return true;
}

// a default implementation for primitive shapes. Compound shapes should implement their own
Intersect	IObject::Intersection(const Ray& ray) const
{
	Ray transformed;
	transformed.origin = InverseTransformPoint(ray.origin, transform);
	transformed.direction = InverseTransformVector(ray.direction, transform);
	
	std::vector<double> dists = findDistances(transformed);
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
	
	out = glm::rotateY(p, glm::radians(t.rotation.y));
	out = glm::rotateX(out, glm::radians(t.rotation.x));
	out = glm::rotateZ(out, glm::radians(t.rotation.z));

	out = out + t.position;
	
	return out;
}

glm::dvec3 IObject::TransformVector(const glm::dvec3& v, const Transform& t)
{
	glm::dvec3 out;

	out = glm::rotateY(v, glm::radians(t.rotation.y));
	out = glm::rotateX(out, glm::radians(t.rotation.x));
	out = glm::rotateZ(out, glm::radians(t.rotation.z));

	return out;
}

glm::dvec3 IObject::InverseTransformPoint(const glm::dvec3& p, const Transform& t)
{
	glm::dvec3 out;

	out = p - t.position;
	
	out = glm::rotateZ(out, glm::radians(-t.rotation.z));
	out = glm::rotateX(out, glm::radians(-t.rotation.x));
	out = glm::rotateY(out, glm::radians(-t.rotation.y));

	return out;
}

glm::dvec3 IObject::InverseTransformVector(const glm::dvec3& v, const Transform& t)
{
	glm::dvec3 out;

	out = glm::rotateZ(v, glm::radians(-t.rotation.z));
	out = glm::rotateX(out, glm::radians(-t.rotation.x));
	out = glm::rotateY(out, glm::radians(-t.rotation.y));

	return out;
}

Transform IObject::CompoundTransform(const Transform& t1, const Transform& t2)
{
	Transform out;

	out.position = TransformPoint(t1.position, t2);

	glm::dmat4 m1 = glm::eulerAngleYXZ(t1.rotation.y, t1.rotation.x, t1.rotation.z);
	glm::dmat4 m2 = glm::eulerAngleYXZ(t2.rotation.y, t2.rotation.x, t2.rotation.z);
	glm::dmat4 m = m2 * m1;
	if (m[0][0] == 1.0)
	{
		out.rotation.y = std::atan2(m[0][2], m[2][3]);
		out.rotation.x = 0;
		out.rotation.z = 0;
	}
	else if (m[0][0] == -1.0)
	{
		out.rotation.y = std::atan2(m[0][2], m[2][3]);
		out.rotation.x = 0;
		out.rotation.z = 0;
	}
	else
	{
		out.rotation.y = std::atan2(-m[2][0], m[0][0]);
		out.rotation.x = glm::asin(m[1][0]);
		out.rotation.z = std::atan2(-m[1][2], m[1][1]);
	}
	return out;
}
