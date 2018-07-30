#include <QSlider>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QFileDialog>
#include <QColorDialog>
#include <QtConcurrent>
#include <QFuture>
#include <QTabWidget>
#include <QCheckBox>

#include "Window.h"
#include "Raytan.hpp"
#include "RenderPipeline.hpp"
#include "lodepng.h"

Window::Window(Scene* s, Camera* c) :
	QWidget(), _layout(), _scene(s), _camera(c),
	_image(NULL), _watcher(NULL), _label(), _progressBar()
{
	setLayout(&_layout);

	// Create right column
	QWidget* rightColumn = new QWidget();
	QVBoxLayout* l = new QVBoxLayout();
	l->setAlignment(Qt::AlignTop);
	rightColumn->setLayout(l);


	QLabel* bouncesLabel = new QLabel(tr("Maximum Ray Bounces"));
	l->addWidget(bouncesLabel);

	QSlider* bouncesSlider = new QSlider(Qt::Horizontal);
	bouncesSlider->setTickInterval(5);
	bouncesSlider->setSingleStep(1);
	bouncesSlider->setMaximum(20);
	bouncesSlider->setMinimum(0);
	l->addWidget(bouncesSlider);
	connect(bouncesSlider, &QAbstractSlider::sliderReleased, this, [=]() {
		_bounces = bouncesSlider->value();
	});
	bouncesSlider->setValue(10);
	_bounces = 10;

	QCheckBox* aa = new QCheckBox(tr("Supersample"));
	l->addWidget(aa);
	// Render Button
	QPushButton* renderButton = new QPushButton(tr("Render"));
	connect(renderButton, &QPushButton::clicked, aa, [=]() {
		if (aa->isChecked())
			render(2048, 2048);
		else
			render(1024, 1024);
	});
	l->addWidget(renderButton);

	// Ambient Light Picker
	QPushButton* ambientButton = new QPushButton(tr("Set Ambient Light"));
	connect(ambientButton, &QPushButton::clicked, [this]() {
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

	QLabel* filter_label = new QLabel(tr("Filters"));
	l->addWidget(filter_label);

	// Filter Tabs
	QTabWidget* filters = new QTabWidget(this);
	l->addWidget(filters);

	// Cartoon
	QWidget* cartoon = new QWidget();
	QVBoxLayout* cartoon_layout = new QVBoxLayout();
	cartoon_layout->setAlignment(Qt::AlignTop);
	cartoon->setLayout(cartoon_layout);
	QCheckBox* cartoon_enabled = new QCheckBox(tr("Enable"));
	cartoon_layout->addWidget(cartoon_enabled);
	filters->addTab(cartoon, tr("Cartoon"));
	connect(cartoon_enabled, &QCheckBox::clicked, this, [=]{
		_filters.cartoon.enabled = cartoon_enabled->isChecked();
	});

	QLabel* cartoon_palette_label = new QLabel(tr("Palette Size"));
	cartoon_layout->addWidget(cartoon_palette_label);
	QSlider* cartoon_palette = new QSlider(Qt::Horizontal);
	cartoon_palette->setTickInterval(5);
	cartoon_palette->setSingleStep(1);
	cartoon_palette->setMaximum(32);
	cartoon_palette->setMinimum(2);
	cartoon_layout->addWidget(cartoon_palette);
	connect(cartoon_palette, &QAbstractSlider::sliderReleased, this, [=]() {
		_filters.cartoon.palette = cartoon_palette->value();
	});

	// Tint
	QWidget* tint = new QWidget();
	QVBoxLayout* tint_layout = new QVBoxLayout();
	tint_layout->setAlignment(Qt::AlignTop);
	tint->setLayout(tint_layout);
	QCheckBox* tint_enabled = new QCheckBox(tr("Enable"));
	tint_layout->addWidget(tint_enabled);
	filters->addTab(tint, tr("Tint"));
	connect(tint_enabled, &QCheckBox::clicked, this, [=]{
		_filters.tint.enabled = tint_enabled->isChecked();
	});
	QPushButton* tint_color = new QPushButton(tr("Set Tint Color"));
	connect(tint_color, &QPushButton::clicked, this, [=]{
		QColor newColor = QColorDialog::getColor(
			QColor(255, 255, 255),
			this,
			tr("Tint Color")
		);
		newColor.getRgbF(&_filters.tint.color.x, &_filters.tint.color.y, &_filters.tint.color.z);
	});
	tint_layout->addWidget(tint_color);

	QLabel* tint_sat_label = new QLabel(tr("Tint Saturation"));
	tint_layout->addWidget(tint_sat_label);
	QSlider* tint_sat = new QSlider(Qt::Horizontal);
	tint_sat->setTickInterval(5);
	tint_sat->setSingleStep(1);
	tint_sat->setMaximum(100);
	tint_sat->setMinimum(0);
	tint_layout->addWidget(tint_sat);
	connect(tint_sat, &QAbstractSlider::sliderReleased, this, [=]() {
		_filters.tint.saturation = 0.01 * tint_sat->value();
	});

	// Edge
	QWidget* edge = new QWidget();
	QVBoxLayout* edge_layout = new QVBoxLayout();
	edge_layout->setAlignment(Qt::AlignTop);
	edge->setLayout(edge_layout);
	QCheckBox* edge_enabled = new QCheckBox(tr("Enable"));
	edge_layout->addWidget(edge_enabled);
	filters->addTab(edge, tr("Edge"));
	connect(edge_enabled, &QCheckBox::clicked, this, [=]{
		_filters.edge.enabled = edge_enabled->isChecked();
	});
	QPushButton* edge_color = new QPushButton(tr("Set Edge Color"));
	connect(edge_color, &QPushButton::clicked, this, [=]{
		QColor newColor = QColorDialog::getColor(
			QColor(255, 255, 255),
			this,
			tr("Edge Color")
		);
		newColor.getRgbF(&_filters.edge.color.x, &_filters.edge.color.y, &_filters.edge.color.z);
	});
	edge_layout->addWidget(edge_color);


	// Motion Blur
	QWidget* motion = new QWidget();
	QVBoxLayout* motion_layout = new QVBoxLayout();
	motion_layout->setAlignment(Qt::AlignTop);
	motion->setLayout(motion_layout);
	QCheckBox* motion_enabled = new QCheckBox(tr("Enable"));
	motion_layout->addWidget(motion_enabled);
	filters->addTab(motion, tr("Motion Blur"));
	connect(motion_enabled, &QCheckBox::clicked, this, [=]{
		_filters.motion.enabled = motion_enabled->isChecked();
	});

	// Anaglyph
	QWidget* anaglyph = new QWidget();
	QVBoxLayout* anaglyph_layout = new QVBoxLayout();
	anaglyph_layout->setAlignment(Qt::AlignTop);
	anaglyph->setLayout(anaglyph_layout);
	QCheckBox* anaglyph_enabled = new QCheckBox(tr("Enable"));
	anaglyph_layout->addWidget(anaglyph_enabled);
	filters->addTab(anaglyph, tr("Anaglyph"));
	connect(anaglyph_enabled, &QCheckBox::clicked, this, [=](){
		_filters.anaglyph.enabled = anaglyph_enabled->isChecked();
	});

	// Progress bar
	_progressBar.setMinimum(0);
	_progressBar.setMaximum(100);
	_progressBar.setValue(0);

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
		RenderPipeline::SceneToImage(_scene, _camera, im, this, _bounces);
		RenderPipeline::NormalizeColor(im, 0.5, 1);
		if (_filters.cartoon.enabled)
			RenderPipeline::Cartoon(im, _filters.cartoon.palette);
		if (_filters.tint.enabled)
			RenderPipeline::Tint(im, _filters.tint.color, _filters.tint.saturation);
		if (_filters.edge.enabled)
			RenderPipeline::SobelEdge(im, _filters.edge.color);
		if (_filters.motion.enabled)
			RenderPipeline::MotionBlur(im);
		if (_filters.anaglyph.enabled)
			RenderPipeline::Anaglyph(im);
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
	_label.setPixmap(QPixmap::fromImage(qim.scaledToWidth(1024, Qt::SmoothTransformation)));
	if (_image)
		delete _image;
	_image = im;
	_progressBar.setValue(0);
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
