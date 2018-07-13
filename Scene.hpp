/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/11 15:51:41 by bpierce           #+#    #+#             */
/*   Updated: 2018/07/12 21:22:34 by bpierce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

struct		Light
{
	glm::dvec3	position;
	glm::dvec3	color;
};

struct  RayResult
{
    glm::dvec3  position;
    glm::dvec3  normal;
    glm::dvec3  color;
    double		refractiveIndex;
    double		diffuse;
    double		reflect;
    double		refract;
};

struct	Ray
{
	glm::dvec3 origin;
	glm::dvec3 direction;
	double refractiveIndex;
};

#define IS_INFIN(a) (a.x == INFINITY)

class Scene
{
	std::vector<IObject>	_objects;
	std::vector<Light>		_lights;

	RawPixel	getRayResult(const Ray & ray, const Intersect & intersect) const;
	RawPixel	getDiffuse(const Ray & ray, const Intersect & intersect) const;
	Ray			getRefract(const Ray & ray, const Intersect & intersect) const;
	Ray			getReflect(const Ray & ray, const Intersect & intersect) const;

	bool		hasShadow(const Ray & ray, const glm::dvec3 & lightPos, double distance) const;

	public:

		Scene(void);
		virtual ~Scene(void);

		RawPixel	CastRay(const Ray & ray, int recursionLevel) const;
}
