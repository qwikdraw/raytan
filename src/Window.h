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

struct Image;
class Window : public QWidget
{
	Q_OBJECT
	QGridLayout _layout;
	Scene& _scene;	
	Camera& _camera;
	Image* _image;
	QFutureWatcher<Image*>* _watcher;
	QLabel _label;
	QProgressBar _progressBar;
	unsigned _bounces = 20;
	void	render(int width = 1024, int height = 1024);
	void    saveRender(void);
public slots:
	void	setImage(void);
signals:
	void	progressUpdate(int);
public:
	explicit Window(Scene& s, Camera& c);
};
