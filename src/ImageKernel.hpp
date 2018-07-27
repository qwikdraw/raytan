#pragma once

#include "Raytan.hpp"

template <size_t S>
struct Kernel
{
	double arr[S][S];
};

template <size_t S>
Image	*ApplyKernel(const Image& image, const Kernel<S>& kernel)
{
	static_assert(S % 2, "Kernal must have odd dimensions");
	
	Image* out = new Image();
	out->width = image.width;
	out->height = image.height;
	out->raw.resize(out->width * out->height);
	for (int x = 0; x < image.width; x++)
	{
		for (int y = 0; y < image.height; y++)
		{
			glm::dvec3 accumalate = glm::dvec3(0);

			for (int Kx = -(int)S/2; Kx <= (int)S/2; Kx++)
			{
				for (int Ky = -(int)S/2; Ky <= (int)S/2; Ky++)
				{
					size_t x_index = glm::clamp(x + Kx, 0, image.width - 1);
					size_t y_index = glm::clamp(y + Ky, 0, image.height - 1);

					accumalate += kernel.arr[Kx + S/2][Ky + S/2] *
						image.raw[x_index + y_index * image.width].color;
				}
			}
			out->raw[x + y * out->width].color = accumalate;
		}
	}
	return out;
}
