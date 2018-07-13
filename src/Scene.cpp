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
	glm::dvec3 bestPos;
	
	for (int i = 0; i < _objects.size(); i++)
	{
		glm::dvec3 p = _objects[i]->Intersection(ray);
		glm::dvec3 l = p - ray.origin;
		double distSqr = glm::dot(l, l);

		if (distSqr < bestDist)
		{
			bestDist = distSqr;
			bestIndex = i;
			bestPos = p;
		}
	}

	if (bestIndex == -1)
	{
		RayResult out;
		out.position = glm::dvec3(INFINITY);
		return out;
	}
	
	return _objects[bestIndex]->MakeRayResult(bestPos, ray);
}

// getDiffuse will also do shadow management

RawColor	Scene::getDiffuse(const Ray & ray, const RayResult & rayResult) const
{
	RawColor 	pixelColor = {{0.0, 0.0, 0.0}, 0};
	Ray		lightRay;
	glm::dvec3	distanceV;
	double		dotValue;

	if (IS_INFIN(rayResult.position)) // necessary? rayResult.position.x?
		return (RawColor){{0.0, 0.0, 0.0}, 0.0};
	for (auto & light : _lights)
	{
		distanceV = light.position - rayResult.position;
		lightRay.direction = glm::normalize(distanceV);
		lightRay.origin = rayResult.position + (lightRay.direction * 0.00001); // Offset
		
		if (!hasShadow(lightRay, light.position, glm::dot(distanceV, distanceV)))
		{
			dotValue = glm::dot(rayResult.normal, lightRay.direction);
			if (dotValue < 0.0)
				dotValue = 0.0;
			pixelColor.color += (rayResult.color * dotValue);
			pixelColor.color += (light.color * dotValue);
		}
	}
	return pixelColor;
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
