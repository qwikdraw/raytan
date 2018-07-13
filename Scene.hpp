/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scene.class.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bpierce <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/11 15:51:41 by bpierce           #+#    #+#             */
/*   Updated: 2018/07/11 16:06:50 by bpierce          ###   ########.fr       */
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
    double  refractiveIndex;
};

class Scene
{
	std::vector<IObject>	_objects;
	std::vector<Light>		_lights;

	Ray			getReflect(const Ray & ray, const Intersect & intersect);
	// Ray		getRefract(const Ray & ray, const Intersect & intersect);
	RawPixel	getDiffuse(const Ray & ray, const Intersect & intersect);

	bool		hasShadow(const Ray & ray, const glm::dvec3 & lightPos, double distance) const;

	public:

		Scene(void);
		virtual ~Scene(void);


		RawPixel	CastRay(const Ray & ray, int recursionLevel);

}
