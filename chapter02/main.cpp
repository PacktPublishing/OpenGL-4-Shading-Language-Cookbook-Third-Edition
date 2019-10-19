#include "scene.h"
#include "scenerunner.h"
#include "scenebasic_attrib.h"
#include "scenebasic_uniform.h"
#include "scenebasic_uniformblock.h"
#include "sceneseparable.h"

#include <memory>

static std::map< std::string, std::string > sceneData = {
		{"basic-attrib", "Prints active attributes."},
		{"basic-uniform", "Basic scene with a uniform variable."},
		{"basic-uniform-block", "Scene with a uniform block variable."},
		{"separable", "Scene using separable shaders and program pipelines."}
};

int main(int argc, char *argv[])
{
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneData);

	SceneRunner runner("Chapter 2 - " + recipe, 500, 500);

	std::unique_ptr<Scene> scene;
	if( recipe == "separable" ) {
		scene = std::unique_ptr<Scene>(new SceneSeparable());
	} else if( recipe == "basic-attrib") {
		scene = std::unique_ptr<Scene>(new SceneBasic_Attrib());
	} else if( recipe == "basic-uniform") {
		scene = std::unique_ptr<Scene>(new SceneBasic_Uniform());
	} else if( recipe == "basic-uniform-block" ) {
		scene = std::unique_ptr<Scene>(new SceneBasic_UniformBlock());
	}

	return runner.run(std::move(scene));
}


