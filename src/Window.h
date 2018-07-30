#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QProgressBar>
#include <QFutureWatcher>

#include "Scene.hpp"
#include "Camera.hpp"
#include "RenderPipeline.hpp"

struct Filters
{
	struct cartoon {
		bool enabled = false;
		int palette = 5;
	} cartoon;
	struct tint {
		bool enabled = false;
		glm::dvec3 color = glm::dvec3(1, 0.8, 0.7);
		double saturation = 1.0;
	} tint;
	struct edge {
		bool enabled = false;
		glm::dvec3 color;
	} edge;
	struct motion {
		bool enabled;
		double distance;
	} motion;
	struct anaglyph {
		bool enabled;
	} anaglyph;
};

struct Image;
class Window : public QWidget
{
	Q_OBJECT
	QGridLayout _layout;
	Scene* _scene;
	Camera* _camera;
	Image* _image;
	QFutureWatcher<Image*>* _watcher;
	QLabel _label;
	QProgressBar _progressBar;
	unsigned _bounces;
	Filters _filters;
	void	render(int width, int height);
	void    saveRender(void);
public slots:
	void	setImage(void);
signals:
	void	progressUpdate(int);
public:
	explicit Window(Scene* s, Camera* c);
};
