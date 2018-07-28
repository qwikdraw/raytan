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
		if (win)
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

void	RP::Cartoon(Image* image, int palette_size)
{
	for (auto& raw : image->raw)
	{
		double grey = (raw.color.r + raw.color.g + raw.color.b) / 3.0;
		raw.color *= (glm::round(grey * palette_size) / palette_size) / grey;
	}
}

void	RP::Anaglyph(Image* image)
{
	int shift = image->width / 200;

	Image rightShift;
	Image leftShift;

	rightShift.width = image->width - shift;
	rightShift.height = image->height;
	leftShift.width = image->width - shift;
	leftShift.height = image->height;
	rightShift.raw.resize(rightShift.width * rightShift.height);
	leftShift.raw.resize(rightShift.width * rightShift.height);

	for (int x = 0; x < image->width; x++)
	{
		for (int y = 0; y < image->height; y++)
		{
			if (x - shift > 0)
			{
				leftShift.raw[x - shift + y * leftShift.width].color =
					image->raw[x + y * image->width].color;
			}
			if (x + shift < image->width)
			{
				rightShift.raw[x + y * rightShift.width].color =
					image->raw[x + y * image->width].color;
			}
		}
	}

	// now to combine right and left shift into a single image

	image->width = rightShift.width;
	image->height = rightShift.height;
	image->raw.resize(rightShift.width * rightShift.height);
	
	for (size_t i = 0; i < (size_t)rightShift.height * rightShift.width; i++)
	{
		image->raw[i].color =
			glm::dvec3(leftShift.raw[i].color.r, 0, 0) +
			glm::dvec3(0, rightShift.raw[i].color.g, rightShift.raw[i].color.b);
	}
}

void	RP::Tint(Image* image, glm::dvec3 color, double saturation)
{
	for (auto& raw : image->raw)
	{
		double grey = (raw.color.r + raw.color.g + raw.color.b) / 3.0;
		glm::dvec3 adjustedColor = grey * color;
		raw.color = raw.color * (1 - saturation) + adjustedColor * saturation;
	}
}

void	RP::MotionBlur(Image* image, double distance)
{
	int shift = image->width * distance;
	Image temp;
	temp.width = image->width;
	temp.height = image->height;
	temp.raw.resize(temp.height * temp.width);

	for (auto& raw : temp.raw)
		raw.color = glm::dvec3(0);
	
	for (int x = 0; x < image->width; x++)
	{
		for (int y = 0; y < image->height; y++)
		{
			double depthShift = shift / (image->raw[x + y * image->width].depth + 0.1);
			glm::dvec3 colShare = image->raw[x + y * image->width].color / depthShift;

			for (int s = -depthShift/2; s < depthShift - depthShift/2; s++)
			{
				if (x + s < 0 || x + s >= image->width)
					continue;
				temp.raw[x + s + y * image->width].color += colShare;
			}
		}
	}
	for (int x = 0; x < image->width; x++)
		for (int y = 0; y < image->height; y++)
			image->raw[x + y * image->width].color = temp.raw[x + y * image->width].color;
}
