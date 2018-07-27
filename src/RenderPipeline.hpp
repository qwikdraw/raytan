#pragma once

#include <stdint.h>

#include "Raytan.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Window.h"
#include "ImageKernel.hpp"

class Window;

namespace	RenderPipeline
{
	void    RenderSquare(const Scene* scene, const Camera& camera, Image* image, Window* win,
		int recursionLvl, int startx, int starty);

	// fills the image with raw color values
	void	SceneToImage(const Scene*, const Camera&, Image* im, Window* win, int recursionLvl = 10);

	// normalizes the raw color values so they lie between 0 and 1
	void	NormalizeColor(Image*, double gamma, double exposure = 1);

	// converts the raw color values to 32 bit rgba values
	void	ImageToRGB32(Image*);

	// uses the depth value of the image to draw edges onto the image
	void	SobelEdge(Image*, glm::dvec3 color = glm::dvec3(1));

	// uses a limited color palette
	void	Cartoon(Image*, int palette_size = 5);

	// makes the image 3D. Note: this destroys the depth so use after filters that require it
	void	Anaglyph(Image*);

	// tints the image with color. Saturation of 0 is no tint. Saturation of 1 is full tint.
	void	Tint(Image*, glm::dvec3 color, double saturation);

	// applies a horizontal blur on the image as if each object moved 'distance' amount
	void	MotionBlur(Image*, double distance = 0.05);
};
