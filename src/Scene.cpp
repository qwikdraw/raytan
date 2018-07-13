/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/11 16:08:11 by bpierce           #+#    #+#             */
/*   Updated: 2018/07/12 21:23:28 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scene.hpp"

#include "Sphere.hpp"

Scene::Scene(void)
{
	_objects.push_back(new Sphere(glm::dvec3(1, 0, 0), 0.2));
	_lights.push_back((Light){{0, 0, 0}, {0, 1, 1}});
}

Scene::~Scene(void)
{
}

RayResult	Scene::getRayResult(const Ray& ray) const
{
	double bestDist = INFINITY;
	int bestIndex = -1;
	
	for (int i = 0; i < _objects.size(); i++)
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

	return _objects[bestIndex]->MakeRayResult(dist, ray);
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
				return glm::vec3(0, 0, 0);
			intensity *= data.refract * data.diffuse * data.color;
		}
	}
	return intentisy;
}

Ray	Scene::getRefract(const Ray & ray, const RayResult & rayResult) const
{
	return (Ray){{0, 0, 0}, {0, 0, 0}, 0};
}

Ray	Scene::getReflect(const Ray & ray, const RayResult & rayResult) const
{
	return (Ray){{0, 0, 0}, {0, 0, 0}, 0};
}

//	Iterates through each object,
//	returns true if the ray hits one object,
//	returns false if no objects were hit.

bool	Scene::hasShadow(const Ray & ray, const glm::dvec3 & lightPos, double distance) const
{
	glm::dvec3	intersect;
	glm::dvec3	shadowV;
	double		shadowDist;

	for (auto object : _objects)
	{
		intersect = object->Intersection(ray);
		if (!IS_INFIN(intersect))
		{
			shadowV = intersect - lightPos;
			shadowDist = glm::dot(shadowV, shadowV);
			if (shadowDist < distance)
				return true;
		}
	}
	return false;
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
	RawColor diffusePart = getDiffuse(ray, rayResult);

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
