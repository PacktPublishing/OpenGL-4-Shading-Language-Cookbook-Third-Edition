Example code from the [OpenGL 4 Shading Language Cookbook, 3rd Edition][cookbook]
=========================================================

The example code from the [OpenGL 4 Shading Language Cookbook, 3rd Edition][cookbook],
by David Wolff and published by Packt Publishing.

Requirements
-------------
To compile these examples, you'll need the following:

* The [GLM Mathematics Library][GLM] version 0.9.6 or later.  Note that versions
  prior to 0.9.6 may not work properly because of a switch from degrees to
  radians.  GLM 0.9.5 will work, but you'll need to add `#define GLM_FORCE_RADIANS`
  prior to including the glm header files.
* [GLFW][] version 3.0 or later.

Compiling the examples
----------------------
The example code builds with [CMake][].  Note that the
examples for Chapter 10 will not function on MacOS due to lack of support for
compute shaders on that platform.

1.  Install [GLFW][] by following the instructions on their [web site][GLFW].
2.  Install the latest version of [GLM][].  Note that for [CMake][] to find GLM
    correctly, you need to run the install "build" (e.g. `make install`) or install GLM from your
    favorite package manager.  Otherwise, the CMake config files will not be created/available.
3.  Download this example code from [github][ghcookbook], or clone using git.
4.  Run cmake.  If cmake has difficulties finding the GLFW or GLM installations,
    set the variable [`CMAKE_PREFIX_PATH`][cmake_prefix] to help cmake find them.  
    It can be tricky to get CMake to find the GLM libraries, unfortunately.  See
    below for tips.
5.  Compile by running `make`.

Any problems, [create an issue](https://github.com/PacktPublishing/OpenGL-4-Shading-Language-Cookbook-Third-Edition/issues) on [github][ghcookbook].

Tips for getting CMake to find GLM
-----------------------------------------
When searching for GLM, CMake looks for the files `glmConfig.cmake` and `glmConfigVersion.cmake`.

If you install GLM using a package manager such as Homebrew on macOS, or a Linux package manager the
 cmake files should already be included.  

Otherwise, if you're using the GLM source distribution, you'll have to run GLM through CMake to get it to
generate the `glmConfig.cmake` and `glmConfigVersion.cmake` files.  It is frustrating, I wish that the GLM 
maintainers didn't require this.

1.  Download GLM and extract it to some location: `GLM_SRC`
1. `cd $GLM_SRC`
1. `mkdir build`
1. `cd build`
1. `cmake -D GLM_TEST_ENABLE=OFF -D CMAKE_INSTALL_PREFIX=MY_GLM_LOCATION ..`
1. `cmake --build . --target install`

Replace `GLM_SRC` above with the place where you extracted the GLM zip file, and replace `MY_GLM_LOCATION` 
with the location where you want to install GLM. This should generate the needed cmake files and install
all of GLM to `MY_GLM_LOCATION`.

Tips for compiling for Windows with Visual Studio
---------------------------------------------
* Use the Visual Studio target in [CMake][]:  `-G "Visual Studio..."`, open the
  Visual Studio solution.  You should see one project per chapter.
* Each chapter requires a command line argument to choose a recipe.  When
  running in VS, be sure to set the 'Command Argument' under 'Properties' for
  the appropriate recipe.

OpenGL Function Loading
-----------------------

An OpenGL header file and a function loader for a 4.3 core profile are
included with this project.  They were generated using
[GLAD][].  This loader should also work on MacOS under a 4.1 core profile, but of course not all functions will load.

The code has been tested with OpenGL 4.3 on Windows/Linux and OpenGL 4.1 on MacOS.

[GLM]: http://glm.g-truc.net
[GLFW]:  http://glfw.org
[ghcookbook]:  https://github.com/PacktPublishing/OpenGL-4-Shading-Language-Cookbook-Third-Edition
[cookbook]: http://www.packtpub.com/
[GLLoadGen]:  https://bitbucket.org/alfonse/glloadgen/wiki/Home
[CMake]: http://www.cmake.org/
[GLAD]: https://github.com/Dav1dde/glad
[cmake_prefix]: https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html
