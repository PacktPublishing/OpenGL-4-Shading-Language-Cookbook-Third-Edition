#include "scene.h"
#include "scenerunner.h"
#include "scenetexture.h"
#include "scenealphatest.h"
#include "scenemultitex.h"
#include "scenenormalmap.h"
#include "sceneprojtex.h"
#include "scenereflectcube.h"
#include "scenerefractcube.h"
#include "scenerendertotex.h"
#include "scenesamplerobj.h"
#include "scenediffibl.h"
#include "sceneparallax.h"

#include <functional>
#include <unordered_map>

std::map<std::string, std::string> sceneInfo = {
	{ "alpha-test", "Discard fragments based on an alpha test." },
	{ "multi-tex", "Multiple textures" },
	{ "normal-map", "Normal map" },
	{ "proj-tex", "Projected texture" },
	{ "reflect-cube", "Reflection with a cube map" },
	{ "refract-cube", "Refraction with a cube map" },
	{ "render-to-tex", "Render to a texture using framebuffer objects" },
	{ "sampler-obj", "Sampler objects " },
	{ "texture", "Basic texture mapping" },
	{"diff-ibl", "Diffuse image based lighting"},
	{"parallax", "Parallax mapping"}
};

static std::unique_ptr<Scene> createScene(const std::string &recipe) {
	using SceneCreator = std::function<std::unique_ptr<Scene>()>;
	static const std::unordered_map<std::string, SceneCreator> sceneCreators = {
		{"alpha-test", [] { return std::unique_ptr<Scene>(new SceneAlphaTest()); }},
		{"multi-tex", [] { return std::unique_ptr<Scene>(new SceneMultiTex()); }},
		{"normal-map", [] { return std::unique_ptr<Scene>(new SceneNormalMap()); }},
		{"proj-tex", [] { return std::unique_ptr<Scene>(new SceneProjTex()); }},
		{"reflect-cube", [] { return std::unique_ptr<Scene>(new SceneReflectCube()); }},
		{"refract-cube", [] { return std::unique_ptr<Scene>(new SceneRefractCube()); }},
		{"render-to-tex", [] { return std::unique_ptr<Scene>(new SceneRenderToTex()); }},
		{"sampler-obj", [] { return std::unique_ptr<Scene>(new SceneSamplerObj()); }},
		{"texture", [] { return std::unique_ptr<Scene>(new SceneTexture()); }},
		{"diff-ibl", [] { return std::unique_ptr<Scene>(new SceneDiffIbl()); }},
		{"parallax", [] { return std::unique_ptr<Scene>(new SceneParallax()); }},
	};

	auto it = sceneCreators.find(recipe);
	return (it == sceneCreators.end()) ? nullptr : it->second();
}

int main(int argc, char *argv[])
{

	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneInfo);

	SceneRunner runner("Chapter 5 - " + recipe, 1024, 678, 4);
	std::unique_ptr<Scene> scene = createScene(recipe);
	if (!scene) {
		printf("Unknown recipe: %s\n", recipe.c_str());
		exit(EXIT_FAILURE);
	}

    return runner.run(std::move(scene));
}
