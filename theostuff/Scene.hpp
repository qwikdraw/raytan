#pragma once

#include "Raetan.hpp"

class	Scene
{
	public:
	RawColor TraceRay(const Ray& ray, int) const
	{
		RawColor out;
		out.color = glm::normalize(ray.direction);
		out.color += glm::dvec3(1);
		out.color /= 2;
		return out;
	}
};
