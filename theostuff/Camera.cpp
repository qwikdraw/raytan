#include "Camera.hpp"

Camera::Camera(glm::dvec3 pos, glm::dvec3 dir, glm::dvec3 up, double fov, double aspect) :
_position(pos)
{
	glm::dmat4 lookAt = glm::lookAt(pos, pos + dir, up);
	glm::dmat4 perspective = glm::perspective(glm::radians(fov), aspect, 0.1, 100.0);

	_deviceToWorld = glm::inverse(perspective * lookAt);
}

Ray	Camera::GetRay(double x, double y) const
{
	glm::dvec4 p = _deviceToWorld * glm::dvec4(x, y, 0, 1);
	p /= p.w;

	Ray out;
	
	out.origin = _position;
	out.direction = glm::normalize(glm::dvec3(p) - _position);
	out.refractiveIndex = 1;
	return out;
}
