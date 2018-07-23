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
	scene.lights.push_back((Light){{-1, 0, 0.5}, {1, 1, 1}});

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
	m2.diffuse = 0.1;
	m2.reflect = 0.9;
	m2.refract = 0;
	m2.refractiveIndex = 1.4;
	m2.materialSampler = nullptr;
	m2.color = glm::dvec3(1, 1, 1);
	m2.colorSampler = nullptr;
	m2.normalSampler = nullptr;

	Material m3;
	m3.diffuse = 0.5;
	m3.reflect = 0;
	m3.refract = 0.5;
	m3.refractiveIndex = 1.1;
	m3.materialSampler = nullptr;
	m3.color = glm::dvec3(1, 0.4, 0.6);
	m3.colorSampler = nullptr;
	m3.normalSampler = nullptr;
	
	Sphere *s1 = new Sphere;
	s1->transform.position = glm::dvec3(0, 0, 0);
	s1->transform.rotation = glm::dvec3(0, 0, 0);
	s1->material = m3;
	
	s1->radius = 0.1;

	Cone *cone = new Cone;
	cone->transform.position = glm::dvec3(0, 0, 0);
	cone->transform.rotation = glm::dvec3(0, 0, 0);
	cone->material = m3;

	cone->angle = 40;

	Plane *plane = new Plane;
	plane->transform.position = glm::dvec3(0, 0, 0);
	plane->transform.rotation = glm::dvec3(0, 0, 0);
	plane->material = m3;
	
	Cylinder *cylinder = new Cylinder;
	cylinder->transform.position = glm::dvec3(0, 0, 0);
	cylinder->transform.rotation = glm::dvec3(90, 0, 0);
	cylinder->material = m3;

	cylinder->radius = 0.05;

	Subtraction *sub1 = new Subtraction(s1, cylinder);
	sub1->transform.position = glm::dvec3(0, 0, 0);
	sub1->transform.rotation = glm::dvec3(0, 0, 0);

	Subtraction *sub2 = new Subtraction(sub1, plane);
	sub2->transform.position = glm::dvec3(0, 0, 0);
	sub2->transform.position = glm::dvec3(0, 0, 0);
	
	Subtraction *sub3 = new Subtraction(sub2, cone);
	sub3->transform.position = glm::dvec3(0, -0.1, 0.1);
	sub3->transform.rotation = glm::dvec3(0, 20, 50);	

	scene.AddObject(sub3);

	Plane *p1 = new Plane;
	p1->transform.position = glm::dvec3(1, 0, 0);
	p1->transform.rotation = glm::dvec3(0, 0, 90);
	p1->material = m1;

	Sphere *s2 = new Sphere;
	s2->transform.position = glm::dvec3(0, 0, 0);
	s2->transform.rotation = glm::dvec3(0, 0, 0);
	s2->material = m2;
	s2->radius = 0.115;
	
	Cube *cube = new Cube;
	cube->transform.position = glm::dvec3(0, 0, 0);
	cube->transform.rotation = glm::dvec3(0, 0, 0);
	cube->size = glm::dvec3(0.2);
	cube->material = m2;

	Subtraction *sub4 = new Subtraction(cube, s2);
	sub4->transform.position = glm::dvec3(0, 0, -0.2);
	sub4->transform.rotation = glm::dvec3(20, 50, 30);
	
	scene.AddObject(sub4);
	
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
	
	glm::dvec3 pos = {-1.3, 0, 0};
	glm::dvec3 dir = {1, 0, 0};
	Camera camera(pos, glm::normalize(dir), glm::dvec3(0, 1, 0), 45, 1);
	
	// Initialize Qt Application with the scene and camera
	QApplication qt(argc, argv);
	Window window(scene, camera);
	window.show();
	return qt.exec();
}
