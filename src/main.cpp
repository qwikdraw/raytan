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
	char	*output = const_cast<char*>("render.png");

	opterr = 0;
	while ((c = getopt (argc, argv, "hdo:")) != -1)
		switch (c)
		{
			case 'h':
				std::cout << "usage: ./RT [-d, -h] <scenefile> -o <outfile>\n" <<
					" -d	Run detached.\n" <<
					" -o	Output file. Only applicable to detached mode.\n" <<
					" -h	This usage information.\n";
				return 1;
			case 'd':
				headless = true;
				break;
			case 'o':
				output = optarg;
				break;
			case '?':
				if (optopt == 'o')
					std::cerr << "Option -" << static_cast<char>(optopt)
						<< " requires a filepath argument" << std::endl;
				else
					std::cerr << "usage error, unknown option: -" 
						<< static_cast<char>(optopt) << std::endl;
					std::cout << "usage: ./RT [-d, -h] <scenefile> -o <outfile>";
				return 1;
			default:
				abort();
		}
	rt = ParseSceneFile((optind < argc) ? argv[optind] : "scene.json");

	if (!headless)
	{
		// Qt GUI
		QApplication qt(argc, argv);
		Window window(&rt->scene, &rt->camera);
		window.show();
		return qt.exec();
	}

	Image* image = new Image(2048, 2048);
	RenderPipeline::SceneToImage(&rt->scene, &rt->camera, image, nullptr, 10);
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
	lodepng::save_file(buffer, output);
	free(rt);
	return 0;
}
