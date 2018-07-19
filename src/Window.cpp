#include "Window.h"
#include "Raytan.hpp"
#include "RenderPipeline.hpp"
#include <QSlider>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QFileDialog>
#include "lodepng.h"

Window::Window(Scene& s, Camera& c) :
	QWidget(), _layout(), _scene(s), _camera(c), _image(NULL), _label(), _progressBar()
{
	setLayout(&_layout);

	// Create right column
	QWidget* rightColumn = new QWidget;
	QVBoxLayout* l = new QVBoxLayout();
	l->setAlignment(Qt::AlignTop);
	rightColumn->setLayout(l);


	QSlider* slider = new QSlider(Qt::Horizontal);
	slider->setTickInterval(10);
	slider->setSingleStep(1);
	l->addWidget(slider);

	// Render Button
	QPushButton* renderButton = new QPushButton(tr("Render"));
	connect(renderButton, &QPushButton::clicked, [this]{
		render();
	});
	l->addWidget(renderButton);

	// Save Button
	QPushButton* saveButton = new QPushButton(tr("Save Render"));
	connect(saveButton, &QPushButton::clicked, [this]{
		saveRender();
	});
	l->addWidget(saveButton);

	_progressBar.setMaximum(100);
	_progressBar.setMinimum(0);

	// Main layout
	_layout.addWidget(&_label, 0, 0);
	_layout.addWidget(rightColumn, 0, 1);
	_layout.addWidget(&_progressBar, 1, 0);
}

void	Window::render(int width, int height)
{
	if (_image)
		delete	_image;
	
	_progressBar.setValue(0);
	_image = new Image(width, height);
	RenderPipeline::SceneToImage(_scene, _camera, _image, _progressBar, 10);
	RenderPipeline::NormalizeColor(_image, 0.5);
	RenderPipeline::ImageToRGB32(_image);
	QImage qim(_image->colors.data(), _image->width, _image->height, QImage::Format_RGB32);
	_label.setPixmap(QPixmap::fromImage(qim));
	_progressBar.setValue(100);
}

void	Window::saveRender(void)
{
	if (!_image)
		return;
	lodepng::State state;
	state.encoder.text_compression = 1;
	state.decoder.color_convert = 0;
	std::string filename = QFileDialog::getSaveFileName(this, tr("Save File"), "render", tr("PNG (*.png)")).toStdString();
	std::vector<uint8_t> buffer;
	unsigned error = lodepng::encode(buffer, _image->colors, _image->width, _image->height, state);
	if(error)
	{
		std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return;
	}
	lodepng::save_file(buffer, filename);
}
