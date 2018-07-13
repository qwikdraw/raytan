#pragma once

#include "Raetan.hpp"

class	Scene
{
	public:
	RawColor TraceRay(const Ray& ray, int) const
	{
		RawColor out;
		out.color = glm::normalize(ray.direction);
		return out;
	}
};
