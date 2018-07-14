
#include "Raetan.hpp"
#include "ImagePipeline.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "Image2D.hpp"

int	main(void)
{
	Scene scene;

	glm::dvec3 pos = {0, 0, 0};
	glm::dvec3 dir = {1, 0, 0};
	Camera camera(pos, dir, glm::dvec3(0, 1, 0), 45, 1);
	
	Image im(1000, 1000);

	ImagePipeline::SceneToImage(scene, camera, im, 2);
	ImagePipeline::Normalize(im, 0.5);
	ImagePipeline::Finalize(im);	

	Window window(1000, 1000, "Raetan");
	Image2D imageDisplay;
	imageDisplay.Render(im.colors, im.width, im.height);

	while (!window.ShouldClose())
	{
		window.Clear();
		imageDisplay.Render();
		window.Render();
	}
	window.Close();
}
