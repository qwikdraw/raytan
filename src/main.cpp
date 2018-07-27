#include <QApplication>
#include "json.hpp"

#include "Window.h"
#include "Raytan.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "parse.hpp"

using json = nlohmann::json;

int	main(int argc, char **argv)
{
	Scene* scene;

	if (argc == 2 && argv[1][0] != '-')
		scene = ParseSceneFile(argv[1]);
	else
		scene = ParseSceneFile("-");
	
	glm::dvec3 pos = {-1.8, 0, 0};
	glm::dvec3 dir = {1, 0, 0};
	Camera camera(pos, glm::normalize(dir), glm::dvec3(0, 1, 0), 45, 16.0 / 9.0);

	if (1)
	{
		// Qt GUI
		QApplication qt(argc, argv);
		Window window(scene, camera);
		window.show();
		return qt.exec();
	}

	Image* image = new Image(1920, 1080);
	RenderPipeline::SceneToImage(scene, camera, image, nullptr, 30);
	RenderPipeline::NormalizeColor(image, 0.5, 1);
	RenderPipeline::ImageToRGB32(image);
	lodepng::State state;
    state.encoder.text_compression = 1;
    state.decoder.color_convert = 0;
    std::vector<uint8_t> buffer;
    unsigned error = lodepng::encode(buffer, image->colors, image->width, image->height, state);
    if(error)
    {
        std::cout << "encoder error: " << lodepng_error_text(error) << std::endl;
        return 1;
    }
    lodepng::save_file(buffer, "render.png");
	return 0;
}
