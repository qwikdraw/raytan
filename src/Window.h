#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QProgressBar>

#include "Scene.hpp"
#include "Camera.hpp"
#include "RenderPipeline.hpp"

class Window : public QWidget
{
	Q_OBJECT
	QGridLayout	_layout;
	Scene&	_scene;	
	Camera&	_camera;
	Image*	_image;
	QLabel	_label;
	QProgressBar _progressBar;
	void	render(int width = 1024, int height = 1024);
	void    saveRender(void);
public:
	explicit Window(Scene& s, Camera& c);
};
