
#include "Ray.hpp"
#include "RawPixel.hpp"

#include "Raetan.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "Image2D.hpp"


// just making a test scene to check if rendering works
class	Scene
{
public:
	RawPixel CastRay(const Ray& ray, int)
	{
		RawPixel out;
		out.color = glm::normalize(ray.direction);
		return out;
	}
};

int	main(void)
{
	Scene scene;

	glm::dvec3 pos = {0, 0, 0};
	glm::dvec3 dir = {1, 0, 0};
	Camera camera(pos, dir, glm::dvec3(0, 1, 0), 80, 1);
	
	std::vector<unsigned char> image(1000 * 1000 * 4);

	for (int x = 0; x < 1000; x++)
	{
		for (int y = 0; y < 1000; y++)
		{
			double normalizedX = (x - 500) / 500.0;
			double normalizedY = (y - 500) / 500.0;
			RawPixel p = scene.CastRay(camera.GetRay(normalizedX, normalizedY), 10);

			image[(x + 1000 * y) * 4 + 0] = (p.color.r + 1) * 120;
			image[(x + 1000 * y) * 4 + 1] = (p.color.g + 1) * 120;
			image[(x + 1000 * y) * 4 + 2] = (p.color.b + 1) * 120;
			image[(x + 1000 * y) * 4 + 3] = 255;
		}
	}

	Window window(1000, 1000, "Raetan");
	Image2D imageDisplay;
	imageDisplay.Render(image, 1000, 1000);

	while (!window.ShouldClose())
	{
		window.Clear();
		imageDisplay.Render();
		window.Render();
	}
	window.Close();
}
