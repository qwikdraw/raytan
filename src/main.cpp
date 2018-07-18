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
	scene.lights.push_back((Light){{0, -0.5, 0.5}, {4, 4, 4}});

	Sphere *s1 = new Sphere;
	s1->center = glm::dvec3(1, 0, 0.1);
	s1->direction = glm::dvec3(0, 0, 1);

	s1->radius = 0.2;
	s1->color = glm::dvec3(1, 1, 1);
	s1->refractiveIndex = 1.0;
	s1->diffuse = 1;
	s1->reflect = 0;
	s1->refract = 0;
	s1->color = glm::dvec3(0.9, 0.5, 0.8);

	s1->colorSampler = new Sampler("assets/image.png");
	s1->materialSampler = nullptr;//new Sampler("mat.png");
	s1->normalSampler = nullptr;

	Plane *p2 = new Plane;
        p2->center = glm::dvec3(1, 0, 0.1);
        p2->direction = glm::normalize(glm::dvec3(1, 0, 2));

        p2->refractiveIndex = 1.0;
        p2->diffuse = 1;
        p2->reflect = 0;
        p2->refract = 0;
        p2->color = glm::dvec3(1, 0, 0.5);

        p2->colorSampler = nullptr;
        p2->materialSampler = nullptr;
        p2->normalSampler = nullptr;

	Subtraction *sub = new Subtraction(s1, p2);

	scene.AddObject(sub);

	Plane *p1 = new Plane;
	p1->center = glm::dvec3(2, 0, 0);
	p1->direction = glm::dvec3(1, 0, 0);

	p1->refractiveIndex = 2;
	p1->diffuse = 1;
	p1->reflect = 0;
	p1->refract = 0;
	p1->color = glm::dvec3(1, 1, 0.5);

	p1->colorSampler = nullptr;
	p1->materialSampler = nullptr;
	p1->normalSampler = nullptr;

	scene.AddObject(p1);

	Cylinder *c1 = new Cylinder;
	c1->center = glm::dvec3(2, 0.3, 0.3);
	c1->radius = 0.2;
	c1->vector = glm::normalize(glm::dvec3(0.1, 0.3, 0.7));
	c1->color = glm::dvec3(1, 1, 1);
	c1->refractiveIndex = 2;
	c1->diffuse = 1;
	c1->reflect = 0;
	c1->refract = 0;
	c1->colorSampler = nullptr;
	c1->materialSampler = nullptr;
	c1->normalSampler = nullptr;

	scene.AddObject(c1);	
}

int	main(int argc, char *argv[])
{
	/*
	** Later the camera a scene will be read from
	** the scene file here.
	*/
	Scene scene;

	add_to_scene(scene);
	
	glm::dvec3 pos = {-1.3, 0, 0.4};
	glm::dvec3 dir = {1, 0, 0};
	Camera camera(pos, glm::normalize(dir), glm::dvec3(0, 1, 0), 45, 1);
	
	// Initialize Qt Application with the scene and camera
	QApplication qt(argc, argv);
	Window window(scene, camera);
	window.show();
	return qt.exec();
}
