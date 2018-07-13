#include "ImagePipeline.hpp"

namespace IP = ImagePipeline;

void	IP::SceneToImage(const Scene& scene, const Camera& camera, Image& image, int recursionLvl)
{
	for (int y = 0; y < image.height; y++)
	{
		for (int x = 0; x < image.width; x++)
		{
			double normalizedX = ((double)x - image.width / 2.0) / (image.width / 2.0);
			double normalizedY = ((double)y - image.height / 2.0) / (image.height / 2.0);
			RawColor c = scene.TraceRay(camera.GetRay(normalizedX, normalizedY), recursionLvl);

			image.raw.push_back(c);
		}
	}
}

void	IP::Normalize(Image& image, double gamma)
{
	glm::dvec3 averageCol;
	double totalPixels = image.width * image.height;
	
	for (auto& raw : image.raw)
	{
		raw.color = glm::pow(raw.color, glm::dvec3(gamma));
		averageCol += raw.color / totalPixels;
	}
	
	glm::dvec3 scaleFactor = averageCol * 2.0;
	
	for (auto& raw : image.raw)
	{
		raw.color.r = std::min(raw.color.r / scaleFactor.r, 1.0);
		raw.color.g = std::min(raw.color.g / scaleFactor.g, 1.0);
		raw.color.b = std::min(raw.color.b / scaleFactor.b, 1.0);
	}
}

void	IP::Finalize(Image& image)
{
	for (auto& raw : image.raw)
	{
		image.colors.push_back(raw.color.r * 255);
		image.colors.push_back(raw.color.g * 255);
		image.colors.push_back(raw.color.b * 255);
		image.colors.push_back(255);
	}
	image.raw.clear();
}
