#pragma once

#include "Raetan.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

struct	Image
{
	Image() {}
	Image(int w, int h) : width(w), height(h) {}
	
	int width;
	int height;
	std::vector<RawColor> raw;
	std::vector<unsigned char> colors;
};

namespace	ImagePipeline
{
	// fills the image with raw color values
	void	SceneToImage(const Scene&, const Camera&, Image&, int recursionLvl = 10);

	// normalizes the raw color values so they lie between 0 and 1
	void	Normalize(Image&, double gamma);

	// converts the raw color values to rgba unsigned char values
	void	Finalize(Image&);
};
