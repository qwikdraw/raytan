#pragma once

#include <stdint.h>

#include "Raytan.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Window.h"

class Window;
struct	Image
{
	Image() {}
	Image(int w, int h) : width(w), height(h) {}
	
	int width;
	int height;
	std::vector<RawColor> raw;
	std::vector<uint8_t> colors;
};

namespace	RenderPipeline
{
	void    RenderSquare(const Scene& scene, const Camera& camera, Image* image, Window* win,
		int recursionLvl, int startx, int starty);

	// fills the image with raw color values
	void	SceneToImage(const Scene&, const Camera&, Image* im, Window* win, int recursionLvl = 10);

	// normalizes the raw color values so they lie between 0 and 1
	void	NormalizeColor(Image*, double gamma);

	// converts the raw color values to 32 bit rgba values
	void	ImageToRGB32(Image*);
};
