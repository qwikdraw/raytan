#include "RenderPipeline.hpp"
#include <iostream>

namespace RP = RenderPipeline;

void	RP::SceneToImage(const Scene& scene, const Camera& camera, Image* image, QProgressBar& pro, int recursionLvl)
{
	for (int y = 0; y < image->height; y++)
	{
		for (int x = 0; x < image->width; x++)
		{
			double normalizedX = (2 * x - image->width) / (double)image->width;
			double normalizedY = (2 * y - image->height) / (double)image->height;
			RawColor c = scene.TraceRay(camera.GetRay(normalizedX, normalizedY), recursionLvl);
			image->raw.push_back(c);
		}
		pro.setValue(1 + 100.0 * (y / (double)image->height));
	}
}

void	RP::Normalize(Image* image, double gamma)
{
	glm::dvec3 averageCol;
	double totalPixels = image->width * image->height;
	
	for (auto& raw : image->raw)
	{
		raw.color = glm::pow(raw.color, glm::dvec3(gamma));
		averageCol += raw.color / totalPixels;
	}
	
	double scaleFactor = (averageCol.r + averageCol.g + averageCol.b) * 2 / 3.0;

	for (auto& raw : image->raw)
	{
		raw.color.r = std::min(raw.color.r / scaleFactor, 1.0);
		raw.color.g = std::min(raw.color.g / scaleFactor, 1.0);
		raw.color.b = std::min(raw.color.b / scaleFactor, 1.0);
	}
}

void	RP::Finalize(Image* image)
{
	for (auto& raw : image->raw)
	{
		image->colors.push_back(raw.color.b * 255);
		image->colors.push_back(raw.color.g * 255);
		image->colors.push_back(raw.color.r * 255);
		image->colors.push_back(255);
	}
/*
	for (auto raw = image.raw.end(); raw != image.raw.begin();)
	{
	    --raw;
		image.colors.push_back(raw->color.b * 255);
		image.colors.push_back(raw->color.g * 255);
		image.colors.push_back(raw->color.r * 255);
		image.colors.push_back(255);
	}
*/
	image->raw.clear();
}
