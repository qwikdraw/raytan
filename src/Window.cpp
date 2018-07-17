#include "Window.h"
#include "Raetan.hpp"
#include "RenderPipeline.hpp"
#include <QSlider>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPixmap>

Window::Window(Scene& s, Camera& c) :
	QWidget(), _layout(), _scene(s), _camera(c), _image(NULL), _label(), _progressBar()
{
	setLayout(&_layout);

	QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setTickInterval(10);
    slider->setSingleStep(1);
	
	QPushButton* button = new QPushButton("Render");

	_layout.addWidget(&_label, 0, 0);

	QWidget* rightColumn = new QWidget;
	QVBoxLayout* l = new QVBoxLayout();
	l->addWidget(slider);
	l->addWidget(button);
	l->setAlignment(Qt::AlignTop);
	rightColumn->setLayout(l);

	_progressBar.setMaximum(100);
	_progressBar.setMinimum(0);

	_layout.addWidget(rightColumn, 0, 1);
	_layout.addWidget(&_progressBar, 1, 0);

	connect(button, &QPushButton::clicked, [this]{
		render();
	});
	render(100, 100);
}

void	Window::render(int width, int height)
{
	if (_image)
		delete	_image;
	
	_progressBar.setValue(0);
	_image = new Image(width, height);
	RenderPipeline::SceneToImage(_scene, _camera, _image, _progressBar, 10);
	RenderPipeline::Normalize(_image, 0.5);
	RenderPipeline::Finalize(_image);

	QImage qim(_image->colors.data(), _image->width, _image->height, QImage::Format_RGB32);

	_label.setPixmap(QPixmap::fromImage(qim));
	_progressBar.setValue(0);
}
