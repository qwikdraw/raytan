/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/11 16:08:11 by bpierce           #+#    #+#             */
/*   Updated: 2018/07/21 18:27:49 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scene.hpp"

Scene::Scene(void)
{
	_ambient = glm::dvec3(0.0);
}

Scene::~Scene(void)
{
}

RayResult	Scene::getRayResult(const Ray& ray) const
{
	std::pair<double, IObject*> bestPair(INFINITY, nullptr);
	int bestIndex = -1;
	
	for (unsigned i = 0; i < _objects.size(); i++)
	{
		auto distPair = _objects[i]->Intersection(ray);

		if (distPair.first < bestPair.first)
		{
			bestPair = distPair;
			bestIndex = i;
		}
	}

	if (bestIndex == -1)
	{
		// a position containing infinity is a failed result
		RayResult out;
		out.position = glm::dvec3(INFINITY);
		return out;
	}

	return _objects[bestIndex]->MakeRayResult(bestPair.first, ray, bestPair.second);
}

// getDiffuse will also do shadow management

RawColor	Scene::getDiffuse(const Ray& ray, const RayResult& rayResult) const
{
	RawColor 	pixelColor = {{0.0, 0.0, 0.0}, 0};
	Ray		lightRay;
	glm::dvec3	lightVector;

	for (auto& light : lights)
	{
		double offset = 0.00001;
		if (glm::dot(ray.direction, rayResult.normal) > 0)
			offset = -offset;
		lightVector = light.position - rayResult.position;
		lightRay.direction = glm::normalize(lightVector);
		lightRay.origin = rayResult.position + (rayResult.normal * offset);

		glm::dvec3 intensity = lightIntensity(lightRay, light, glm::length(lightVector));
		intensity *= glm::abs(glm::dot(rayResult.normal, lightRay.direction));
		intensity = glm::max(intensity, _ambient);
		
		pixelColor.color += rayResult.color * intensity;
	}
	return pixelColor;
}

glm::dvec3	Scene::lightIntensity(const Ray& ray, const Light& light, double lightDist) const
{
	// inverse square falloff
	glm::dvec3 intensity = light.color / (lightDist * lightDist);

	for (auto object : _objects)
	{
		auto pair = object->Intersection(ray);
		double dist = pair.first;
		if (dist < lightDist)
		{
			RayResult data = object->MakeRayResult(dist, ray, pair.second);
			if (data.refract == 0)
				return glm::dvec3(0, 0, 0);
			intensity *= data.refract * (1.0 - data.diffuse * (glm::dvec3(1) - data.color));
		}
	}
	return intensity;
}

static glm::dvec3 refract(const glm::dvec3& I, const glm::dvec3& N, double eta)
{
	double NdotI = glm::dot(N, I);
	double cosi = -glm::clamp(NdotI, -1.0, 1.0);	
	double k = 1.0 - eta * eta * (1.0 - cosi * cosi);

	if (k < 0.0)
		return glm::reflect(I, N);
	return eta * I + (eta * cosi - glm::sqrt(k)) * N;
}

Ray	Scene::getRefract(const Ray & ray, const RayResult & rayResult) const
{
	glm::dvec3 normal;
	double ratio;
	
	if (glm::dot(ray.direction, rayResult.normal) > 0)
	{
		normal = rayResult.normal * -1.0;
		ratio = rayResult.refractiveIndex;
	}
	else
	{
		normal = rayResult.normal;
		ratio = 1.0 / rayResult.refractiveIndex;
	}

	Ray out;
	out.direction = glm::normalize(refract(ray.direction, normal, ratio));
	out.origin = rayResult.position + out.direction * 0.00001; // Offset
	return out;
}

Ray	Scene::getReflect(const Ray & ray, const RayResult & rayResult) const
{
	Ray out;
	out.direction = glm::normalize(glm::reflect(ray.direction, rayResult.normal));
	out.origin = rayResult.position + out.direction * 0.00001; // Offset
	return out;
}

RawColor	Scene::TraceRay(const Ray & ray, int recursionLevel) const
{
	if (recursionLevel == -1)
		return (RawColor){{0.0, 1.0, 0.0}, INFINITY};

	// The point of Intersection
	RayResult rayResult = getRayResult(ray);
	if (IS_INFIN(rayResult.position))
		return (RawColor){{0.0, 0.0, 0.0}, INFINITY};

	// The diffuse color
	RawColor diffusePart = {{0.0, 0.0, 0.0}, 0};
	if (rayResult.diffuse > 0)
	{
		diffusePart = getDiffuse(ray, rayResult);
	}

 	// The reflect color
	RawColor reflectPart = {{0.0, 0.0, 0.0}, 0};
 	if (rayResult.reflect > 0)
 	{
		Ray reflection = getReflect(ray, rayResult);
		reflectPart = TraceRay(reflection, recursionLevel - 1);
 	}

 	// The refract color
	RawColor refractPart = {{0.0, 0.0, 0.0}, 0};
 	if (rayResult.refract > 0)
 	{
		Ray refraction = getRefract(ray, rayResult);
		refractPart = TraceRay(refraction, recursionLevel - 1);
	}

	// Output color
	RawColor output;
	output.depth = glm::length(rayResult.position - ray.origin);
	output.color = diffusePart.color * rayResult.diffuse +
		       reflectPart.color * rayResult.reflect +
		       refractPart.color * rayResult.refract;
	return output;
}

void	Scene::SetAmbient(glm::dvec3 color)
{
	_ambient = color;
}

void	Scene::AddObject(IObject* o)
{
	_objects.push_back(o);
}

void	Scene::RemoveObject(IObject* o)
{
	for (size_t i = 0; i < _objects.size(); i++)
	{
		if (_objects[i] == o)
		{
			_objects.erase(_objects.begin() + i);
			return;
		}
	}
}
