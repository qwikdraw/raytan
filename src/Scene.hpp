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

#include "Raytan.hpp"
#include "IObject.hpp"

class Scene
{
	std::vector<IObject*> _objects;
	glm::dvec3 _ambient;

	RayResult	getRayResult(const Ray&) const;

	RawColor	getDiffuse(const Ray&, const RayResult&) const;
	glm::dvec3	lightIntensity(const Ray&, const Light&, double lightDist) const;
	
	Ray		getRefract(const Ray&, const RayResult&) const;
	Ray		getReflect(const Ray&, const RayResult&) const;
	glm::dvec3	getDirectLight(const Ray&, const RayResult&) const;

public:

	Scene(void);
	virtual ~Scene(void);

	RawColor	TraceRay(const Ray&, int recursionLevel) const;
	IObject*	GetObject(const Ray&) const;
	void		SetAmbient(glm::dvec3 color);
	void		AddObject(IObject*);
	void		RemoveObject(IObject*);

	// generates a 'parallel' light
	static Light	Parallel(const glm::dvec3& direction, const glm::dvec3& color);
	
	std::vector<Light> lights;
	double lightRadius;
	int lightSample;
};
