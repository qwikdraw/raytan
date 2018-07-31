#include <thread>

#include "RenderPipeline.hpp"

namespace RP = RenderPipeline;

void	RP::RenderSquare(const Scene* scene, const Camera* camera, Image* image, Window* win,
	int recursionLvl, int startx)
{
	for (int y = 0; y < image->height; ++y)
	{
		for (int x = startx; x < image->width; x += 4)
		{
			double normalizedX = (2 * x - image->width) / (double)image->width;
			double normalizedY = (2 * y - image->height) / (double)image->height;
			RawColor c = scene->TraceRay(camera->GetRay(normalizedX, normalizedY), recursionLvl);
			image->raw[x + y * image->width] = c;
		}
		// Only first thread reports progress, which makes progressbar much smoother with little accuracy
		// reduction.
		//if (win && (y + startx) % 4 == 0)
		if (win && !startx)
			emit win->progressUpdate(1 + 100.0 * (y / (double)image->height));
	}
}

void	RP::SceneToImage(const Scene* scene, const Camera* camera, Image* image, Window* win, int recursionLvl)
{
	image->raw.resize(image->width * image->height);
	std::thread one(RP::RenderSquare, scene, camera, image, win, recursionLvl, 0);
	std::thread two(RP::RenderSquare, scene, camera, image, win, recursionLvl, 1);
	std::thread three(RP::RenderSquare, scene, camera, image, win, recursionLvl, 2);
	std::thread four(RP::RenderSquare, scene, camera, image, win, recursionLvl, 3);
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
	std::vector<uint8_t> tmp;
	int x;	

	x = 0;
	for (auto raw = image->raw.end(); raw != image->raw.begin();)
	{
		--raw;
		++x;
		tmp.push_back(255);
		tmp.push_back(raw->color.b * 255);
		tmp.push_back(raw->color.g * 255);
		tmp.push_back(raw->color.r * 255);
		if (x >= image->width)
		{
			std::reverse(tmp.begin(), tmp.end());
			image->colors.insert(image->colors.end(), tmp.begin(), tmp.end());
			tmp.clear();
			x = 0;
		}
	}
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
		raw.color = glm::clamp(raw.color, 0.0, 1.0);
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
		raw.color = glm::clamp(raw.color * (1 - saturation) + adjustedColor * saturation, 0.0, 1.0);
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
			if (!std::isfinite(image->raw[x + y * image->width].depth))
				image->raw[x + y * image->width].depth = 1000;
			double depthShift = glm::max(shift / (image->raw[x + y * image->width].depth + 0.1), 1.0);
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
			image->raw[x + y * image->width].color = glm::clamp(temp.raw[x + y * image->width].color, 0.0, 1.0);
}
