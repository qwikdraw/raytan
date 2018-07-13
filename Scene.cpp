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

// struct Intersect
// {
// 	glm::vec3 position;
// 	glm::vec3 normal;
// 	glm::vec3 color;
// }


// getDiffuse will also do shadow management

RawPixel	Scene::getDiffuse(const Ray & ray, const Intersect & intersect)
{
	RawPixel 	pixelColor;
	RayResult	lightRay;
	glm::dvec3	distanceV;
	double		dotValue;

	if (intersect.position == INFINITY) // necessary? intersect.position.x?
	{
		pixelColor = 0;
		return pixelColor;
	}
	pixelColor.rawColor = 0;
	for (auto & light : _lights)
	{
		distanceV = light.position - intersect.position;
		lightRay.normal = glm::normalize(distanceV);
		lightRay.position = intersect.position + (lightRay.normal * 0.00001); // Offset
		if (!hasShadow(lightRay, light.position, glm::dot(distanceV, distanceV)))
		{
			dotValue = glm::dot(intersect.normal, lightRay.normal);
			if (dotValue < 0.0)
				dotValue = 0.0;
			pixelColor.rawColor += (intersect.color * dotValue);
			pixelColor.rawColor += (light.color * dotValue);
		}
	}
	return pixelColor;
}

//	Iterates through each object,
//	returns true if the ray hits one object,
//	returns false if no objects were hit.

bool	Scene::hasShadow(const Ray & ray, const glm::dvec3 & lightPos, double distance) const
{
	Intersect 	intersect;
	glm::dvec3	shadowV;
	double		shadowDist;

	for (auto & object : _objects)
	{
		intersect = object.intersect(ray);
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

RawPixel	Scene::CastRay(const Ray & ray, int recursionLevel)
{
	(void)ray;
	(void)recursionLevel;
}
