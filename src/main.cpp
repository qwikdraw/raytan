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
//	scene.SetAmbient(glm::dvec3(0.05, 0.05, 0.05));
	scene.lights.push_back((Light){{-1, 0, 0}, {1, 1, 1}});

	Material m1;
	m1.diffuse = 1;
	m1.reflect = 0;
	m1.refract = 0;
	m1.refractiveIndex = 1.4;
	m1.materialSampler = nullptr;
	m1.color = glm::dvec3(1, 1, 1);
	m1.colorSampler = nullptr;//new Sampler("assets/image.png");
	m1.normalSampler = nullptr;

	Material m2;
	m2.diffuse = 1;
	m2.reflect = 0;
	m2.refract = 0;
	m2.refractiveIndex = 1.4;
	m2.materialSampler = nullptr;
	m2.color = glm::dvec3(1, 1, 1);
	m2.colorSampler = nullptr;
	m2.normalSampler = nullptr;

	Material m3;
	m3.diffuse = 1;
	m3.reflect = 0;
	m3.refract = 0;
	m3.refractiveIndex = 1.4;
	m3.materialSampler = nullptr;
	m3.color = glm::dvec3(1, 1, 1);
	m3.colorSampler = nullptr;
	m3.normalSampler = nullptr;
	
	Sphere *s1 = new Sphere;
	s1->transform.position = glm::dvec3(0, 0, 0);
	s1->transform.rotation = glm::dvec3(0, 0, 0);
	s1->material = m1;
	
	s1->radius = 0.1;

	Sphere *s2 = new Sphere;
	s2->transform.position = glm::dvec3(0, 0, 1);
	s2->transform.rotation = glm::dvec3(0, 0, 0);
	s2->material = m1;

	s2->radius = 0.1;

	Plane *p2 = new Plane;
	p2->transform.position = glm::dvec3(0, 0, 0);
	p2->transform.rotation = glm::dvec3(0, 0, 0);
	p2->material = m3;
	
	Subtraction *sub = new Subtraction(s1, s2);
	sub->transform.position = glm::dvec3(0, 0, 0);
	sub->transform.rotation = glm::dvec3(0, 0, 0);

	Subtraction *sub2 = new Subtraction(s1, p2);
	sub2->transform.position = glm::dvec3(0, 0, 0);
	sub2->transform.rotation = glm::dvec3(0, 0, 0);

	scene.AddObject(sub2);

	Plane *p1 = new Plane;
	p1->transform.position = glm::dvec3(1, 0, 0);
	p1->transform.rotation = glm::dvec3(0, 0, 90);
	p1->material = m1;
	
	scene.AddObject(p1);
	
}

int	main(int argc, char *argv[])
{
	/*
	** Later the camera a scene will be read from
	** the scene file here.
	*/
	Scene scene;

	add_to_scene(scene);
	
	glm::dvec3 pos = {-1, 0, 0};
	glm::dvec3 dir = {1, 0, 0};
	Camera camera(pos, glm::normalize(dir), glm::dvec3(0, 1, 0), 45, 1);
	
	// Initialize Qt Application with the scene and camera
	QApplication qt(argc, argv);
	Window window(scene, camera);
	window.show();
	return qt.exec();
}
