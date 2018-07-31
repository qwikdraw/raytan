#include "IObject.hpp"

constexpr glm::dvec3 IObject::direction;

SmallVector<Intersect>	IObject::findIntersections(const Ray&) const
{
	assert(!"default implementation should not be called");
	return SmallVector<Intersect>();
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
	
	SmallVector<double> dists = findDistances(transformed);
	double bestDist = INFINITY;

	for (size_t index = 0; index < dists.size(); index++)
	{
		auto d = dists[index];
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
		double materialSum = sample.r + sample.g + sample.b;
		if (materialSum == 0)
		{
			out.diffuse = 1;
			out.reflect = 0;
			out.refract = 0;
		}
		else
		{
			out.diffuse = sample.r / materialSum;
			out.reflect = sample.g / materialSum;
			out.refract = sample.b / materialSum;
		}
		out.refractiveIndex = 1.0 + sample.a;
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

	if (intersect.hitRef->material.normalSampler)
	{
		glm::dvec3 p = relativePos;//glm::mod(relativePos, 1.0);
		glm::dvec3 sample = glm::dvec3(intersect.hitRef->material.normalSampler->Color(p.x, p.y, p.z));
		sample = sample - glm::dvec3(0.5);
		if (sample.x != 0 || sample.y != 0 || sample.z != 0)
		{
			sample = glm::normalize(sample);
			out.normal = glm::normalize(out.normal + sample  * 0.65);
		}
	}

	if (!intersect.positive)
		out.normal = -out.normal;
	
	return out;
}

glm::dvec3 IObject::TransformPoint(const glm::dvec3& p, const Transform& t)
{
	glm::dvec3 out;
	
	out = glm::rotateZ(p, glm::radians(t.rotation.z));
	out = glm::rotateY(out, glm::radians(t.rotation.y));
	out = glm::rotateX(out, glm::radians(t.rotation.x));

	out = out + t.position;
	
	return out;
}

glm::dvec3 IObject::TransformVector(const glm::dvec3& v, const Transform& t)
{
	glm::dvec3 out;

	out = glm::rotateZ(v, glm::radians(t.rotation.z));
	out = glm::rotateY(out, glm::radians(t.rotation.y));
	out = glm::rotateX(out, glm::radians(t.rotation.x));

	return out;
}

glm::dvec3 IObject::InverseTransformPoint(const glm::dvec3& p, const Transform& t)
{
	glm::dvec3 out;

	out = p - t.position;
	
	out = glm::rotateX(out, glm::radians(-t.rotation.x));
	out = glm::rotateY(out, glm::radians(-t.rotation.y));
	out = glm::rotateZ(out, glm::radians(-t.rotation.z));

	return out;
}

glm::dvec3 IObject::InverseTransformVector(const glm::dvec3& v, const Transform& t)
{
	glm::dvec3 out;

	out = glm::rotateX(v, glm::radians(-t.rotation.x));
	out = glm::rotateY(out, glm::radians(-t.rotation.y));
	out = glm::rotateZ(out, glm::radians(-t.rotation.z));

	return out;
}

Transform IObject::CompoundTransform(const Transform& t1, const Transform& t2)
{
	Transform out;

	out.position = TransformPoint(t1.position, t2);
	
	glm::dmat4 m1 = glm::eulerAngleXYZ(glm::radians(t1.rotation.x),
					   glm::radians(t1.rotation.y),
					   glm::radians(t1.rotation.z));
	glm::dmat4 m2 = glm::eulerAngleXYZ(glm::radians(t2.rotation.x),
					   glm::radians(t2.rotation.y),
					   glm::radians(t2.rotation.z));
	
	glm::extractEulerAngleXYZ(m2 * m1, out.rotation.x, out.rotation.y, out.rotation.z);
	out.rotation = glm::degrees(out.rotation);
	return out;
}
