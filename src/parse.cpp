#include <unordered_map>

#include "parse.hpp"
#include "json.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "Cube.hpp"
#include "Subtraction.hpp"

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
	}}
};

using json = nlohmann::json;

/* Extractors */

static glm::dvec3 get_dvec3(const json& j, std::string key, glm::dvec3 def = glm::dvec3(0.0, 0.0, 0.0))
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

static Material get_material(const json& j, material_map& m)
{
	if (j.count("material") && j["material"].is_string())
	{
		if (m.count(j["material"]) > 0)
			return m[j["material"]];
		if (default_materials.count(j["material"]) > 0)
			return default_materials[j["material"]];
	}
	return default_materials["plastic"];
}

static double get_double(const json& j, std::string key, double def = 0.0)
{
	if (j.count(key) && j[key].is_number())
		return j[key];
	return def;
}

/* Object Parsers */

std::unordered_map<std::string, std::function<IObject*(const json& object)>> object_parsers = {
	{"sphere", [](const json& o) {
		Sphere* tmp = new Sphere;
		tmp->radius = get_double(o, "radius", 0.1);
		return tmp;
	}},
	{"cylinder", [](const json& o) {
		Cylinder* tmp = new Cylinder;
		tmp->radius = get_double(o, "radius", 0.1);
		return tmp;
	}},
	{"cone", [](const json& o) {
		Cone* tmp = new Cone;
		tmp->angle = get_double(o, "angle", 0.1);
		return tmp;
	}},
	{"plane", [](const json&) {
		return new Plane;
	}},
	{"cube", [](const json& o) {
		Cube* tmp = new Cube;
		tmp->size = get_dvec3(o, "size", glm::dvec3(0.1, 0.1, 0.1));
		return tmp;
	}},
};

/* Section Parsers */

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
		tmp.colorSampler = nullptr;
		tmp.materialSampler = nullptr;
		tmp.normalSampler = nullptr;
		materials.insert({m["name"], tmp});
	}
}

static void parseObjects(const json& objects_json, material_map& materials, Scene* scene)
{
	for (auto& o : objects_json)
	{
		if (!o.count("type"))
			continue;
		if (object_parsers.count(o["type"]))
		{
			IObject* tmp = object_parsers[o["type"]](o);
			tmp->transform.position = get_dvec3(o, "position");
			tmp->transform.rotation = get_dvec3(o, "rotation");
			tmp->material = get_material(o, materials);
			scene->AddObject(tmp);
		}
	}
}

/* Parse from a file */

Scene*	ParseSceneFile(std::string sceneFile)
{
	std::ifstream t(sceneFile);
	std::stringstream buffer;
	buffer << t.rdbuf();
	material_map materials;

	Scene* scene = new Scene();
	auto j = json::parse(buffer.str());
	if (!j.count("objects") || !j["objects"].is_array())
		return scene;
	if (j.count("materials") && j["materials"].is_array())
		parseMaterials(j["materials"], materials);
	parseObjects(j["objects"], materials, scene);
	scene->lights.push_back((Light){{-1, 0, 0.5}, {1, 1, 1}});
	return scene;
}