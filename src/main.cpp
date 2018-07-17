#include <QApplication>
#include <QLabel>
#include <QImage>
#include <QPixmap>

#include "Raetan.hpp"
#include "ImagePipeline.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

int	main(int argc, char *argv[])
{
	Scene scene;

	glm::dvec3 pos = {-1.3, 0, 0.4};
	glm::dvec3 dir = {1, 0, 0};
	Camera camera(pos, glm::normalize(dir), glm::dvec3(0, 1, 0), 45, 1);
	
	Image im(1000, 1000);

	ImagePipeline::SceneToImage(scene, camera, im, 10);
	ImagePipeline::Normalize(im, 0.5);
	ImagePipeline::Finalize(im);	

	// QT Stuff.
	QApplication a(argc, argv);
	QImage Result(im.colors.data(), im.width, im.height, QImage::Format_RGB32);
	QLabel label;
	label.setPixmap(QPixmap::fromImage(Result));
	label.show();
	return a.exec();
}
