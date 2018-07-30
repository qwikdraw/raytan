#include "Sampler.hpp"

Sampler::Sampler(const std::string& filepath)
{
	unsigned error = lodepng::decode(_imageData, _width, _height, filepath.c_str());

	if (error)
	{
		std::cout << filepath << std::endl;
		std::cout << "error: " << lodepng_error_text(error) << std::endl;
	}
	_hasFormula2D = false;
	_hasFormula3D = false;
}

Sampler::Sampler(const std::function<glm::dvec4(double, double)>& formula2D) : _formula2D(formula2D)
{
	_hasFormula2D = true;
	_hasFormula3D = false;
}

Sampler::Sampler(const std::function<glm::dvec4(double, double, double)>& formula3D) : _formula3D(formula3D)
{
	_hasFormula2D = false;
	_hasFormula3D = true;
}

glm::dvec4	Sampler::Color(double x, double y) const
{
	if (!_hasFormula2D && !_hasFormula3D)
	{
		glm::dvec4 out;
		unsigned xIndex = glm::round((_width - 1) * glm::clamp(x, 0.0, 1.0));
		unsigned yIndex = glm::round((_height - 1) * glm::clamp(y, 0.0, 1.0));

		out.r = _imageData[(xIndex + _width * yIndex) * 4 + 0];
		out.g =	_imageData[(xIndex + _width * yIndex) * 4 + 1];	
		out.b =	_imageData[(xIndex + _width * yIndex) * 4 + 2];
		out.a = _imageData[(xIndex + _width * yIndex) * 4 + 3];
		return out / 255.0;
	}
	else if (!_hasFormula2D)
		return _formula3D(x, y, 0);
	return _formula2D(x, y);
}

glm::dvec4	Sampler::Color(double x, double y, double z) const
{
	if (_hasFormula3D)
		return _formula3D(x, y, z);
	else if (_hasFormula2D)
		return _formula2D(x, y);

	glm::dvec4 out;
	unsigned xIndex = glm::round((_width - 1) * glm::clamp(x, 0.0, 1.0));
	unsigned yIndex = glm::round((_height - 1) * glm::clamp(y, 0.0, 1.0));

	out.r = _imageData[(xIndex + _width * yIndex) * 4 + 0];
	out.g = _imageData[(xIndex + _width * yIndex) * 4 + 1];
	out.b = _imageData[(xIndex + _width * yIndex) * 4 + 2];
	out.a = _imageData[(xIndex + _width * yIndex) * 4 + 3];
	return out / 255.0;
}
