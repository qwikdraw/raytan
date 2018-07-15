/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/11 16:08:11 by bpierce           #+#    #+#             */
/*   Updated: 2018/07/13 19:36:24 by lkaser           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scene.hpp"

#include "Sphere.hpp"
#include "Plane.hpp"

Scene::Scene(void)
{
	Sphere *s1 = new Sphere;
	s1->center = glm::dvec3(1, 0, 0.1);
	s1->radius = 0.1;
	s1->color = glm::dvec3(1, 1, 1);
	s1->refractiveIndex = 2;
	s1->diffuse = 0.2;
	s1->reflect = 0.4;
	s1->refract = 0.4;
	s1->color = glm::dvec3(0.9, 0.5, 0.8);
	s1->colorSampler.Load("image.png");
	_objects.push_back(s1);

	Plane *p1 = new Plane;
	p1->center = glm::dvec3(2, 0, 0);
	p1->normal = glm::dvec3(1, 0, 0);
	p1->refractiveIndex = 2;
	p1->diffuse = 0.5;
	p1->reflect = 0.5;
	p1->refract = 0;
	p1->color = glm::dvec3(1, 1, 0.5);
//	p1->colorSampler.Load("image.png");
	_objects.push_back(p1);
	
	_lights.push_back((Light){{0, -0.5, 0.5}, {1, 1, 1}});
}

Scene::~Scene(void)
{
}

RayResult	Scene::getRayResult(const Ray& ray) const
{
	double bestDist = INFINITY;
	int bestIndex = -1;
	
	for (unsigned i = 0; i < _objects.size(); i++)
	{
		double dist = _objects[i]->Intersection(ray);

		if (dist < bestDist)
		{
			bestDist = dist;
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

	return _objects[bestIndex]->MakeRayResult(bestDist, ray);
}

// getDiffuse will also do shadow management

RawColor	Scene::getDiffuse(const Ray& ray, const RayResult& rayResult) const
{
	RawColor 	pixelColor = {{0.0, 0.0, 0.0}, 0};
	Ray		lightRay;
	glm::dvec3	lightVector;
	double		dotValue;

	for (auto& light : _lights)
	{
		lightVector = light.position - rayResult.position;
		lightRay.direction = glm::normalize(lightVector);
		lightRay.origin = rayResult.position + (lightRay.direction * 0.00001); // Offset

		glm::dvec3 intensity = lightIntensity(lightRay, light, glm::length(lightVector));
		if (intensity.r == 0 && intensity.g == 0 && intensity.b == 0)
			continue;

		dotValue = std::max(glm::dot(rayResult.normal, lightRay.direction), 0.0);
		pixelColor.color += rayResult.color * intensity * dotValue;
	}	
	return pixelColor;
}

glm::dvec3	Scene::lightIntensity(const Ray& ray, const Light& light, double lightDist) const
{
	// inverse square falloff
	glm::dvec3 intensity = light.color / (lightDist * lightDist);

	for (auto object : _objects)
	{
		double dist = object->Intersection(ray);
		if (dist < lightDist)
		{
			RayResult data = object->MakeRayResult(dist, ray);
			if (data.refract == 0)
				return glm::dvec3(0, 0, 0);
			intensity *= data.refract * (1.0 - data.diffuse * (glm::dvec3(1) - data.color));
		}
	}
	return intensity;
}

Ray	Scene::getRefract(const Ray & ray, const RayResult & rayResult) const
{
	glm::dvec3 normal;
	double ratio;
	
	if (glm::dot(ray.direction, rayResult.normal) > 0)
	{
		normal = rayResult.normal * -1.0;
		ratio = rayResult.refractiveIndex / 1;
	}
	else
	{
		normal = rayResult.normal;
		ratio = 1 / rayResult.refractiveIndex;
	}

	Ray out;
	out.direction = glm::normalize(glm::refract(ray.direction, normal, ratio));
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
		return (RawColor){{0.0, 0.0, 0.0}, INFINITY};

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
