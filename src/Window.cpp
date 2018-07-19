#include <QSlider>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QFileDialog>
#include <QtConcurrent>
#include <QFuture>

#include "Window.h"
#include "Raytan.hpp"
#include "RenderPipeline.hpp"
#include "lodepng.h"

Window::Window(Scene& s, Camera& c) :
	QWidget(), _layout(), _scene(s), _camera(c),
	_image(NULL), _watcher(NULL), _label(), _progressBar()
{
	setLayout(&_layout);

	// Create right column
	QWidget* rightColumn = new QWidget;
	QVBoxLayout* l = new QVBoxLayout();
	l->setAlignment(Qt::AlignTop);
	rightColumn->setLayout(l);


	QLabel* bouncesLabel = new QLabel(tr("Ray Bounces"));
	l->addWidget(bouncesLabel);

	QSlider* bouncesSlider = new QSlider(Qt::Horizontal);
	bouncesSlider->setTickInterval(5);
	bouncesSlider->setSingleStep(1);
	bouncesSlider->setMaximum(100);
	bouncesSlider->setMinimum(0);
	bouncesSlider->setValue(20);
	l->addWidget(bouncesSlider);
	connect(bouncesSlider, &QAbstractSlider::sliderReleased, [this, bouncesSlider]{
		this->_bounces = bouncesSlider->value();
	});

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
	if (_watcher && _watcher->isFinished())
	{
		delete _watcher;
		_watcher = NULL;
	}
	else if (_watcher)
		return;

	QFuture<Image*> renderTask = QtConcurrent::run([this, width, height](){
		Image* im = new Image(width, height);
		RenderPipeline::SceneToImage(_scene, _camera, im, this, _bounces);
		RenderPipeline::NormalizeColor(im, 0.5);
		RenderPipeline::ImageToRGB32(im);
		return im;
	});

	_watcher = new QFutureWatcher<Image*>(this);
	connect(_watcher, SIGNAL(finished()), this, SLOT(setImage()));
	_watcher->setFuture(renderTask);
	connect(this, SIGNAL(progressUpdate(int)), &_progressBar, SLOT(setValue(int)), Qt::QueuedConnection);
}

void	Window::setImage(void)
{
	Image* im = _watcher->result();
	QImage qim(im->colors.data(), im->width, im->height, QImage::Format_RGBA8888);
	_label.setPixmap(QPixmap::fromImage(qim));
	if (_image)
		delete _image;
	_image = im;
}

void	Window::saveRender(void)
{
	if (!_image)
		return;
	lodepng::State state;
	state.encoder.text_compression = 1;
	state.decoder.color_convert = 0;
	std::string filename = QFileDialog::getSaveFileName(this, tr("Save File"), "render", tr("PNG (*.png)")).toStdString();
	if (filename == "")
		return;
	std::vector<uint8_t> buffer;
	unsigned error = lodepng::encode(buffer, _image->colors, _image->width, _image->height, state);
	if(error)
	{
		std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return;
	}
	lodepng::save_file(buffer, filename);
}
