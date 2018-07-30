#include <QApplication>
#include <unistd.h>
#include "json.hpp"

#include "Window.h"
#include "Raytan.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "parse.hpp"

using json = nlohmann::json;

int	main(int argc, char **argv)
{
	RT*		rt;
	bool	headless = false;
	int		c;

	opterr = 0;
	while ((c = getopt (argc, argv, "hc:")) != -1)
		switch (c)
		{
			case 'h':
				headless = true;
				break;
			case '?':
				if (optopt == 'c')
					std::cerr << "Option -" << static_cast<char>(optopt)
						<< " requires an argument" << std::endl;
				else
					std::cerr << "usage error, unknown option: -" 
						<< static_cast<char>(optopt) << std::endl;
				return 1;
			default:
				abort();
		}
	if (optind < argc)
		rt = ParseSceneFile(argv[optind]);
	else
		rt = ParseSceneFile("scene.json");

	if (!headless)
	{
		// Qt GUI
		QApplication qt(argc, argv);
		Window window(&rt->scene, &rt->camera);
		window.show();
		return qt.exec();
	}

	Image* image = new Image(4096, 4096);
	RenderPipeline::SceneToImage(&rt->scene, &rt->camera, image, nullptr, 30);
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
	free(rt);
	return 0;
}
