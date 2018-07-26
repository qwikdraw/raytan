#include <QSlider>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QFileDialog>
#include <QColorDialog>
#include <QtConcurrent>
#include <QFuture>

#include "Window.h"
#include "Raytan.hpp"
#include "RenderPipeline.hpp"
#include "lodepng.h"

Window::Window(Scene* s, Camera& c) :
	QWidget(), _layout(), _scene(s), _camera(c),
	_image(NULL), _watcher(NULL), _label(), _progressBar()
{
	setLayout(&_layout);

	// Create right column
	QWidget* rightColumn = new QWidget;
	QVBoxLayout* l = new QVBoxLayout();
	l->setAlignment(Qt::AlignTop);
	rightColumn->setLayout(l);


	QLabel* bouncesLabel = new QLabel(tr("Maximum Ray Bounces"));
	l->addWidget(bouncesLabel);

	QSlider* bouncesSlider = new QSlider(Qt::Horizontal);
	bouncesSlider->setTickInterval(5);
	bouncesSlider->setSingleStep(1);
	bouncesSlider->setMaximum(60);
	bouncesSlider->setMinimum(0);
	bouncesSlider->setValue(10);
	l->addWidget(bouncesSlider);
	connect(bouncesSlider, &QAbstractSlider::sliderReleased, [this, bouncesSlider]{
		this->_bounces = bouncesSlider->value();
	});

	// Render Button
	QPushButton* renderButton = new QPushButton(tr("Render"));
	connect(renderButton, &QPushButton::clicked, [this]{
		render(2048, 2048);
	});
	l->addWidget(renderButton);

	// Ambient Light Picker
	QPushButton* ambientButton = new QPushButton(tr("Set Ambient Light"));
	connect(ambientButton, &QPushButton::clicked, [this]{
		QColor newAmbient = QColorDialog::getColor(
			QColor(255, 255, 255),
			this,
			tr("Ambient Color")
		);
		glm::dvec3 tmp;
		newAmbient.getRgbF(&tmp.x, &tmp.y, &tmp.z);
		_scene->SetAmbient(tmp);
	});
	l->addWidget(ambientButton);

	// Save Button
	QPushButton* saveButton = new QPushButton(tr("Save Render"));
	connect(saveButton, &QPushButton::clicked, [this]{
		saveRender();
	});
	l->addWidget(saveButton);

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

	_progressBar.setValue(0);
	QFuture<Image*> renderTask = QtConcurrent::run([this, width, height](){
		Image* im = new Image(width, height);
		_progressBar.setMaximum(height * 2);
		RenderPipeline::SceneToImage(_scene, _camera, im, this, _bounces);
		RenderPipeline::NormalizeColor(im, 0.5, 1);
		RenderPipeline::SobelEdge(im);
		RenderPipeline::ImageToRGB32(im);
		return im;
	});

	_watcher = new QFutureWatcher<Image*>(this);
	connect(_watcher, SIGNAL(finished()), this, SLOT(setImage()));
	_watcher->setFuture(renderTask);
	connect(this, SIGNAL(progressUpdate()), this, SLOT(progressStep()), Qt::QueuedConnection);
}

void	Window::progressStep(void)
{
	_progressBar.setValue(_progressBar.value() + 1);
}

void	Window::setImage(void)
{
	Image* im = _watcher->result();
	QImage qim(im->colors.data(), im->width, im->height, QImage::Format_RGBA8888);
	_label.setPixmap(QPixmap::fromImage(qim.scaledToWidth(1000, Qt::SmoothTransformation)));
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
