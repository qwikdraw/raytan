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

#include "Raetan.hpp"
#include "IObject.hpp"

class Scene
{
	std::vector<IObject*> _objects;
	std::vector<Light> _lights;

	RayResult	getRayResult(const Ray&) const;
	RawColor	getDiffuse(const Ray&, const RayResult&) const;
	Ray		getRefract(const Ray&, const RayResult&) const;
	Ray		getReflect(const Ray&, const RayResult&) const;

	bool		hasShadow(const Ray&, const glm::dvec3& lightPos, double distance) const;

	public:

		Scene(void);
		virtual ~Scene(void);

		RawColor	TraceRay(const Ray&, int recursionLevel) const;
};
