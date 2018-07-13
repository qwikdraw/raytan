/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.class.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/11 16:08:11 by bpierce           #+#    #+#             */
/*   Updated: 2018/07/11 16:08:20 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Scene.hpp"


// getDiffuse will also do shadow management

RawColor	Scene::getDiffuse(const Ray & ray, const RayResult & rayResult) const
{
	RawColor 	pixelColor;
	Ray			lightRay;
	glm::dvec3	distanceV;
	double		dotValue;

	if (rayResult.position.x == INFINITY) // necessary? rayResult.position.x?
		return (RawColor){0, 0, 0, 0};
	pixelColor.color = 0;
	for (auto & light : _lights)
	{
		distanceV = light.position - rayResult.position;
		lightRay.normal = glm::normalize(distanceV);
		lightRay.origin = rayResult.position + (lightRay.normal * 0.00001); // Offset
		if (!hasShadow(lightRay, light.position, glm::dot(distanceV, distanceV)))
		{
			dotValue = glm::dot(rayResult.normal, lightRay.normal);
			if (dotValue < 0.0)
				dotValue = 0.0;
			pixelColor.color += (rayResult.color * dotValue);
			pixelColor.color += (light.color * dotValue);
		}
	}
	return pixelColor;
}

RawColor	Scene::getRefract(const Ray & ray, const RayResult & rayResult) const
{
	return (RawColor){0, 0, 0, 0};
}

RawColor	Scene::getReflect(const Ray & ray, const RayResult & rayResult) const
{
	return (RawColor){0, 0, 0, 0};
}

//	Iterates through each object,
//	returns true if the ray hits one object,
//	returns false if no objects were hit.

bool	Scene::hasShadow(const Ray & ray, const glm::dvec3 & lightPos, double distance) const
{
	rayResult 	rayResult;
	glm::dvec3	shadowV;
	double		shadowDist;

	for (auto & object : _objects)
	{
		rayResult = object.intersect(ray);
		if (intersect.position.x != INFINITY)
		{
			shadowV = intersect.position - lightPos;
			shadowDist = glm::dot(shadowV, shadowV);
			if (shadowDist < distance)
				return true;
		}
	}
	return false;
}

RawColor	Scene::CastRay(const Ray & ray, int recursionLevel)
{
	if (recursionLevel == -1)
		return (RawColor){0, 0, 0, INFINITY};

	// The point of Intersection
	RayResult rayResult = getRayResult(ray);
	if (IS_INFIN(rayResult.position))
		return (RawColor){0, 0, 0, INFINITY};

	// The diffuse color
 	RawColor diffusePart = getDiffuse(ray, rayResult);

 	// The reflect color
 	if (rayResult.reflect > 0)
 	{
		Ray reflection = getReflect(ray, rayResult);
		RawColor reflectPart = CastRay(reflection, recursionLevel - 1);
		;
 	}

 	// The refract color
 	if (rayResult.refract > 0)
 	{
		Ray refraction = getRefract(ray, rayResult);
		RawColor refractPart = CastRay(refraction, recursionLevel - 1);
		;
	}

	// Output color
	RawColor output;
	output.depth = glm::length(rayResult.position - ray.origin);
    output.rawColor	= diffusePart.rawColor * rayRes.diffuse +
		 			  reflectPart.rawColor * rayRes.reflect +
		  			  refractPart.rawColor * rayRes.refract;
	return output;
}
