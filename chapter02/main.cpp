#include "scene.h"
#include "scenerunner.h"
#include "sceneseparable.h"

#include <memory>

static std::map< std::string, std::string > sceneData = {
		{"separable", "Scene using separable shaders and program pipelines."}
};

int main(int argc, char *argv[])
{
	std::string recipe = SceneRunner::parseCLArgs(argc, argv, sceneData);

	SceneRunner runner("Chapter 2 - " + recipe, 500, 500);

	std::unique_ptr<Scene> scene;
	if( recipe == "separable" ) {
		scene = std::unique_ptr<Scene>(new SceneSeparable());
	}
	return runner.run(*scene);
}


