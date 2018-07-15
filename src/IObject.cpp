#include "IObject.hpp"

RayResult	IObject::MakeRayResult(double distance, const Ray& ray) const
{
	RayResult out;

	out.position = ray.origin + ray.direction * distance;
	out.normal = normal(distance, ray);

	glm::dvec2 uv;
	if (materialSampler || colorSampler || normalSampler)
		uv = uvMap(out.position, out.normal);
	
	if (materialSampler)
	{
		glm::dvec4 sample = materialSampler->Color(uv.x, uv.y);
		out.diffuse = sample.r;
		out.reflect = sample.g;
		out.refract = sample.b;
		out.refractiveIndex = sample.a;
	}
	else
	{
		out.diffuse = diffuse;
		out.reflect = reflect;
		out.refract = refract;
		out.refractiveIndex = refractiveIndex;
	}

	if (colorSampler)
	{
		glm::dvec4 sample = colorSampler->Color(uv.x, uv.y);
		out.color = sample.xyz;
	}
	else
	{
		out.color = color;
	}

	if (normalSampler)
	{
		glm::dvec4 sample = normalSampler->Color(uv.x, uv.y);
		
	}
}
