#include <thread>

#include "RenderPipeline.hpp"

namespace RP = RenderPipeline;

void	RP::RenderSquare(const Scene* scene, const Camera& camera, Image* image, Window* win,
	int recursionLvl, int startx, int starty)
{
	for (int y = starty; y < starty + image->height / 2; y++)
	{
		for (int x = startx; x < startx + image->width / 2; x++)
		{
			double normalizedX = (2 * x - image->width) / (double)image->width;
			double normalizedY = (2 * y - image->height) / (double)image->height;
			RawColor c = scene->TraceRay(camera.GetRay(normalizedX, normalizedY), recursionLvl);
			image->raw[x + y * image->width] = c;
		}
		emit win->progressUpdate();
	}
}

void	RP::SceneToImage(const Scene* scene, const Camera& camera, Image* image, Window* win, int recursionLvl)
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

void	RP::NormalizeColor(Image* image, double gamma, double exposure)
{
	for (auto& raw : image->raw)
	{
		if (!std::isfinite(raw.color.x) ||
		    !std::isfinite(raw.color.y) ||
		    !std::isfinite(raw.color.z))
		{
			raw.color = glm::dvec3(0);
		}
		raw.color = (raw.color * exposure) / (raw.color * exposure + glm::dvec3(1));
		raw.color = glm::pow(raw.color, glm::dvec3(gamma));
		raw.color = glm::clamp(raw.color, 0.0, 1.0);
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

void	RP::SobelEdge(Image* image, glm::dvec3 color)
{
	Kernel<3> x_kernel = {{{ 3,  0, -3},
			       { 10,  0, -10},
			       { 3,  0, -3}}};
	
	Kernel<3> y_kernel = {{{ 3,  10,  3},
			       { 0,  0,  0},
			       {-3, -10, -3}}};

	Image depth;

	depth.width = image->width;
	depth.height = image->height;
	depth.raw.resize(depth.width * depth.height);

	for (int x = 0; x < image->width; x++)
	{
		for (int y = 0; y < image->height; y++)
		{
			if (!std::isfinite(image->raw[x + y * image->width].depth))
				depth.raw[x + y * image->width].color.x = 1000;
			else
				depth.raw[x + y * image->width].color.x = image->raw[x + y * image->width].depth;
		}
	}

	Image* gx = ApplyKernel(depth, x_kernel);
	Image* gy = ApplyKernel(depth, y_kernel);

	for (int x = 0; x < image->width; x++)
	{
		for (int y = 0; y < image->height; y++)
		{
			size_t index = x + y * image->width;
			double x_val = gx->raw[index].color.x;
			double y_val = gy->raw[index].color.x;
			bool line = (glm::sqrt(x_val * x_val + y_val * y_val) > 0.5);

			if (line)
				image->raw[x + y * image->width].color = color;
		}
	}
	delete gx;
	delete gy;
}
