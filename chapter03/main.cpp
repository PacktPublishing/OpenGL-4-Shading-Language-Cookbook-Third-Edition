#include "scene.h"
#include "scenerunner.h"
#include "scenephong.h"
#include "scenediffuse.h"
#include "scenediscard.h"
#include "sceneflat.h"
#include "scenesubroutine.h"
#include "scenetwoside.h"

#include <map>

static std::map< std::string, std::string > sceneData = {
		{"phong",      "Phong reflection mdoel (per-vertex)."},
		{"diffuse",    "Diffuse shading only"},
		{"discard",    "Example of discarding fragments"},
		{"flat",       "Flat shading"},
		{"subroutine", "Using a shader subroutine"},
		{"two-side",   "Two-sided lighting"}
};

int main( int argc, char ** argv) {

	std::string recipeName = SceneRunner::parseCLArgs(argc, argv, sceneData);
    SceneRunner runner("Chapter 3 - " + recipeName);

	std::unique_ptr<Scene> scene;
	if(recipeName == "phong") {
		scene = std::unique_ptr<Scene>(new ScenePhong());
	}
	else if( recipeName == "diffuse") {
		scene = std::unique_ptr<Scene>(new SceneDiffuse());
	}
	else if( recipeName == "discard" ) {
		scene = std::unique_ptr<Scene>(new SceneDiscard());
	}
	else if( recipeName == "flat" ) {
		scene = std::unique_ptr<Scene>(new SceneFlat());
	}
	else if( recipeName == "subroutine") {
		scene = std::unique_ptr<Scene>(new SceneSubroutine());
	}
	else if( recipeName == "two-side" ) {
		scene = std::unique_ptr<Scene>(new SceneTwoSide());
	}

	return runner.run(*scene);
}