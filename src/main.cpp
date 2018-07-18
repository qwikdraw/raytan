#include <QApplication>

#include "Window.h"
#include "Raytan.hpp"
#include "Scene.hpp"
#include "Camera.hpp"

#include "Sphere.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Cube.hpp"
#include "Subtraction.hpp"

static void	add_to_scene(Scene& scene)
{
	scene.SetAmbient(glm::dvec3(0.05, 0.05, 0.05));
	scene.lights.push_back((Light){{-1, 1, 0.5}, {1, 1, 1}});

	Material m1;
	m1.diffuse = 1;
	m1.reflect = 0;
	m1.refract = 0;
	m1.refractiveIndex = 1.4;
	m1.materialSampler = nullptr;
	m1.color = glm::dvec3(1, 1, 0.5);
	m1.colorSampler = new Sampler("assets/image.png");
	m1.normalSampler = nullptr;

	Sphere *s1 = new Sphere;
	s1->position = glm::dvec3(0, 0, 0);
	s1->rotation = glm::dvec3(0, 0, 0);
	s1->material = m1;
	
	s1->radius = 0.2;

	scene.AddObject(s1);
}

int	main(int argc, char *argv[])
{
	/*
	** Later the camera a scene will be read from
	** the scene file here.
	*/
	Scene scene;

	add_to_scene(scene);
	
	glm::dvec3 pos = {-1.3, 0, 0};
	glm::dvec3 dir = {1, 0, 0};
	Camera camera(pos, glm::normalize(dir), glm::dvec3(0, 1, 0), 45, 1);
	
	// Initialize Qt Application with the scene and camera
	QApplication qt(argc, argv);
	Window window(scene, camera);
	window.show();
	return qt.exec();
}
