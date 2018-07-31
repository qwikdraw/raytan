#include <unordered_map>

#include "parse.hpp"
#include "json.hpp"
#include "Sphere.hpp"
#include "Ellipse.hpp"
#include "Plane.hpp"
#include "Sheet.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Cube.hpp"
#include "Subtraction.hpp"
#include "Addition.hpp"
#include "Sampler.hpp"
#include "Formula.hpp"

/* Samplers */

typedef std::unordered_map<std::string, Sampler> sampler_map;

sampler_map samplers = {
	{"checkerboard", Sampler(Formula2D::checkerboard)},
	{"sineWave", Sampler(Formula2D::sineWave)},
	{"perlinNoise", Sampler(Formula3D::perlinNoise)},
};

//{"customComplicatedAlgorithm", Sampler(Formula2D::customComplicatedAlgorithm)},

/* Default Materials */

typedef std::unordered_map<std::string, Material> material_map;

material_map default_materials = {
	{"glass", {
		.diffuse = 0.01,
		.reflect = 0.0,
		.refract = 0.99,
		.refractiveIndex = 1.5168,
		.color = glm::dvec3(1.0, 1.0, 1.0),
		.colorSampler = nullptr,
		.materialSampler = nullptr,
		.normalSampler = nullptr
	}},
	{"plastic", {
		.diffuse = 0.7,
		.reflect = 0.3,
		.refract = 0.0,
		.refractiveIndex = 0.0,
		.color = glm::dvec3(1.0, 0.3, 0.3),
		.colorSampler = nullptr,
		.materialSampler = nullptr,
		.normalSampler = nullptr
	}},
	{"test", { // For testing samples - will delete later
		.diffuse = 0.9,
		.reflect = 0.1,
		.refract = 0.0,
		.refractiveIndex = 0.0,
		.color = glm::dvec3(1.0, 1.0, 1.0),
		.colorSampler = new Sampler(Formula3D::vortex),
		.materialSampler = nullptr,
		.normalSampler = nullptr//new Sampler(Formula3D::perlinNoise)
	}}
};

using json = nlohmann::json;

/* Extractors */

static glm::dvec3 get_dvec3(const json& j, const std::string key, glm::dvec3 def = glm::dvec3(0.0, 0.0, 0.0))
{
	if (j.count(key) && j[key].is_array() && j[key].size() == 3)
	{
		auto v = j[key];
		if (!(v[0].is_number() && v[1].is_number()  && v[2].is_number()))
			return def;
		return glm::dvec3(v[0], v[1], v[2]);
	}
	return def;
}

static glm::dvec2 get_dvec2(const json& j, const std::string key, glm::dvec2 def = glm::dvec2(0.0, 0.0))
{
	if (j.count(key) && j[key].is_array() && j[key].size() == 2)
	{
		auto v = j[key];
		if (!(v[0].is_number() && v[1].is_number()))
			return def;
		return glm::dvec2(v[0], v[1]);
	}
	return def;
}

static Material get_material(const json& j, const material_map& m)
{
	if (j.count("material") && j["material"].is_string())
	{
		if (m.count(j["material"]) > 0)
			return m.at(j["material"]);
		if (default_materials.count(j["material"]) > 0)
			return default_materials.at(j["material"]);
	}
	return default_materials.at("plastic");
}

static Sampler* get_sampler(const json& j, std::string key)
{
	if (j.count(key) && j[key].is_string())
	{
		if (samplers.count(j[key]) > 0)
			return &samplers.at(j[key]);
		std::string path = j[key];
		if (path.find(".png") != std::string::npos)
		{
			std::cout << path << std::endl;
			return new Sampler(path);
		}
	}
	return nullptr;
}

static double get_double(const json& j, const std::string key, double def = 0.0)
{
	if (j.count(key) && j[key].is_number())
		return j[key];
	return def;
}

typedef std::function<IObject*(const json&, const material_map&)> object_parser;

static bool valid_object(const json& o, const std::string key, std::unordered_map<std::string, object_parser>& parsers)
{
	if (o.count(key) && o[key].is_object()
		&& o[key].count("type") && o[key]["type"].is_string()
		&& parsers.count(o[key]["type"]))
		return true;
	return false;
}

/* Object Parsers */

std::unordered_map<std::string, object_parser> object_parsers = {
	{"sphere", [](const json& o, const material_map&) {
		Sphere* tmp = new Sphere;
		tmp->radius = get_double(o, "radius", 0.1);
		return tmp;
	}},
	{"ellipse", [](const json& o, const material_map&) {
		Ellipse* tmp = new Ellipse;
		tmp->constant = get_dvec3(o, "constant", glm::dvec3(1.0, 1.0, 1.0));
		return tmp;
	}},
	{"cylinder", [](const json& o, const material_map&) {
		Cylinder* tmp = new Cylinder;
		tmp->radius = get_double(o, "radius", 0.1);
		return tmp;
	}},
	{"cone", [](const json& o, const material_map&) {
		Cone* tmp = new Cone;
		tmp->angle = get_double(o, "angle", 0.1);
		return tmp;
	}},
	{"plane", [](const json&, const material_map&) {
		return new Plane;
	}},
	{"cube", [](const json& o, const material_map&) {
		Cube* tmp = new Cube;
		tmp->size = get_dvec3(o, "size", glm::dvec3(0.1, 0.1, 0.1));
		return tmp;
	}},
	{"sheet", [](const json& o, const material_map&) {
		Sheet* tmp = new Sheet;
		tmp->size = get_dvec2(o, "size", glm::dvec2(0.1, 0.1));
		return tmp;
	}},
	{"subtraction", [](const json& o, const material_map& materials) {
		if (!valid_object(o, "positive", object_parsers))
		{
			std::cerr << "Subtraction object requires a valid positive object" << std::endl;
			exit(1);
		}
		IObject* positive = object_parsers[o["positive"]["type"]](o["positive"], materials);
		positive->transform.position = get_dvec3(o["positive"], "position");
		positive->transform.rotation = get_dvec3(o["positive"], "rotation");
		positive->material = get_material(o["positive"], materials);

		if (!valid_object(o, "negative", object_parsers))
		{
			std::cerr << "Subtraction object requires a valid negative object" << std::endl;
			exit(1);
		}
		IObject* negative = object_parsers[o["negative"]["type"]](o["negative"], materials);
		negative->transform.position = get_dvec3(o["negative"], "position");
		negative->transform.rotation = get_dvec3(o["negative"], "rotation");
		negative->material = get_material(o["negative"], materials);
		IObject* tmp = new Subtraction(positive, negative);
		return tmp;
	}},
	{"addition", [](const json& o, const material_map& materials) {
		if (!valid_object(o, "object_a", object_parsers))
		{
			std::cerr << "Addition object requires a valid object_a" << std::endl;
			exit(1);
		}
		IObject* object_a = object_parsers[o["object_a"]["type"]](o["object_a"], materials);
		object_a->transform.position = get_dvec3(o["object_a"], "position");
		object_a->transform.rotation = get_dvec3(o["object_a"], "rotation");
		object_a->material = get_material(o["object_a"], materials);

		if (!valid_object(o, "object_b", object_parsers))
		{
			std::cerr << "Addition object requires a valid object_b" << std::endl;
			exit(1);
		}
		IObject* object_b = object_parsers[o["object_b"]["type"]](o["object_b"], materials);
		object_b->transform.position = get_dvec3(o["object_b"], "position");
		object_b->transform.rotation = get_dvec3(o["object_b"], "rotation");
		object_b->material = get_material(o["object_b"], materials);
		IObject* tmp = new Addition(object_a, object_b);
		return tmp;
	}}
};


/* Section Parsers */


static void	parseScene(const json& scene_json, RT* rt)
{
	rt->scene.SetAmbient(get_dvec3(scene_json, "ambient", glm::dvec3(0.0001)));

	if (scene_json.count("lights") && scene_json["lights"].is_array())
	{
		for (auto& light : scene_json["lights"])
		{
			if (!light.is_object())
			{
				std::cerr << "Invalid light: " << light << std::endl;
				exit(1);
			}
			Light tmp;
			tmp.position = get_dvec3(light, "position");
			tmp.color = get_dvec3(light, "color", glm::dvec3(1.0));
			rt->scene.lights.push_back(tmp);
		}
	}
	if (scene_json.count("camera") && scene_json["camera"].is_object())
	{
		auto& camera_json = scene_json["camera"];
		glm::dvec3 pos = get_dvec3(camera_json, "position", glm::dvec3(-2.0, 0.0, 0.0));
		glm::dvec3 dir = get_dvec3(camera_json, "direction", glm::dvec3(1.0, 0.0, 0.0));
		glm::dvec3 up = get_dvec3(camera_json, "up", glm::dvec3(0.0, 1.0, 0.0));
		double fov = get_double(camera_json, "fov", 45.0);
		double aspectRatio = get_double(camera_json, "aspectRatio", 1.0);
		rt->camera = Camera(pos, dir, up, fov, aspectRatio);
	}
	// Action..
}


static void	parseMaterials(const json& materials_json, material_map& materials)
{
	for (auto& m : materials_json)
	{
		if (!m.count("name") || !m["name"].is_string())
			continue;
		Material tmp;
		tmp.color = get_dvec3(m, "color");
		tmp.diffuse = get_double(m, "diffuse");
		tmp.reflect = get_double(m, "reflect");
		tmp.refract = get_double(m, "refract");
		tmp.colorSampler = get_sampler(m, "color_sampler");
		tmp.materialSampler = get_sampler(m, "material_sampler");
		tmp.normalSampler = get_sampler(m, "normal_sampler");
		tmp.refractiveIndex = get_double(m, "refractiveIndex", 1.0);
		materials.insert({m["name"], tmp});
	}
}

static void parseObjects(const json& objects_json, material_map& materials, Scene* scene)
{
	for (auto& o : objects_json)
	{
		if (!(o.count("type") && o["type"].is_string() && object_parsers.count(o["type"])))
		{
			std::cerr << "Every object must have a valid type" << std::endl;
			std::cerr << "Invalid object: " << o << std::endl;
			exit(1);
		}
		if (object_parsers.count(o["type"]))
		{
			IObject* tmp = object_parsers[o["type"]](o, materials);
			tmp->transform.position = get_dvec3(o, "position");
			tmp->transform.rotation = get_dvec3(o, "rotation");
			tmp->material = get_material(o, materials);
			scene->AddObject(tmp);
		}
	}
}

/* 
** Parse from a file 
** If the scenefile is called `-`
** it reads from standard input.
*/

RT*	ParseSceneFile(std::string sceneFile)
{
	std::stringstream buffer;
	json j;

	if (sceneFile == "-")
	{
		std::string line;
		while (std::getline(std::cin, line))
			buffer << line;
	}
	else
	{
		std::ifstream tmp(sceneFile);
		buffer << tmp.rdbuf();
	}

	try
	{
		j = json::parse(buffer.str());
	}
	catch (std::exception)
	{
		std::cout << "Parse error." << std::endl;
		exit(1);
	}
	material_map materials;
	if (!j.count("objects") || !j["objects"].is_array())
	{
		std::cerr << "Scene requires objects" << std::endl;
		exit(1);
	}

	// Default camera;
    glm::dvec3 pos = {-2.0, 0.0, 0.0};
    glm::dvec3 dir = {1.0, 0.0, 0.0};
	RT* rt = new RT{Camera(pos, dir, glm::dvec3(0, 1, 0), 45, 1.0), Scene()};
	if (j.count("scene") && j["scene"].is_object())
		parseScene(j["scene"], rt);
	if (j.count("materials") && j["materials"].is_array())
		parseMaterials(j["materials"], materials);
	parseObjects(j["objects"], materials, &rt->scene);
	return rt;
}
