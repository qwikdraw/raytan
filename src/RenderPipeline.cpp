#include <thread>

#include "RenderPipeline.hpp"

namespace RP = RenderPipeline;

void	RP::RenderSquare(const Scene& scene, const Camera& camera, Image* image, Window* win,
	int recursionLvl, int startx, int starty)
{
	for (int y = starty; y < starty + image->height / 2; y++)
	{
		for (int x = startx; x < startx + image->width / 2; x++)
		{
			double normalizedX = (2 * x - image->width) / (double)image->width;
			double normalizedY = (2 * y - image->height) / (double)image->height;
			RawColor c = scene.TraceRay(camera.GetRay(normalizedX, normalizedY), recursionLvl);
			image->raw[x + y * image->width] = c;
		}
		emit win->progressUpdate();
	}
}

void	RP::SceneToImage(const Scene& scene, const Camera& camera, Image* image, Window* win, int recursionLvl)
{
	image->raw.resize(image->width * image->height);
	std::thread one(RP::RenderSquare, scene, camera, image, win, recursionLvl, 0, 0);
	std::thread two(RP::RenderSquare, scene, camera, image, win, recursionLvl, image->width / 2, 0);
	std::thread three(RP::RenderSquare, scene, camera, image, win, recursionLvl, image->width / 2, image->height / 2);
	std::thread four(RP::RenderSquare, scene, camera, image, win, recursionLvl, 0, image->height / 2);
	one.join();
	two.join();
	three.join();
	four.join();
}

void	RP::NormalizeColor(Image* image, double gamma)
{
	glm::dvec3 averageCol = glm::dvec3(0);
	double totalPixels = image->width * image->height;
	
	for (auto& raw : image->raw)
	{
		raw.color = glm::pow(raw.color, glm::dvec3(gamma));
		if (std::isfinite(raw.color.x) &&
		    std::isfinite(raw.color.y) &&
		    std::isfinite(raw.color.z))
		{
			averageCol += raw.color / totalPixels;			
		}
	}

	std::cout << averageCol.x << " " << averageCol.y << " " << averageCol.z << std::endl;
	
	double scaleFactor = (averageCol.r + averageCol.g + averageCol.b) * 2 / 3.0;

	for (auto& raw : image->raw)
	{
		raw.color.r = std::min(raw.color.r / scaleFactor, 1.0);
		raw.color.g = std::min(raw.color.g / scaleFactor, 1.0);
		raw.color.b = std::min(raw.color.b / scaleFactor, 1.0);
	}
}

void	RP::ImageToRGB32(Image* image)
{
	for (auto& raw : image->raw)
	{
		image->colors.push_back(raw.color.r * 255);
		image->colors.push_back(raw.color.g * 255);
		image->colors.push_back(raw.color.b * 255);
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
