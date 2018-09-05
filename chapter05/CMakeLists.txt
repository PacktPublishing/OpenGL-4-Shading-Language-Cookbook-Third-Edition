set( target chapter05 )
set( chapter05_SOURCES
	scenetexture.cpp
	main.cpp
	scenerendertotex.cpp
	scenemultitex.cpp
	scenealphatest.cpp
	scenereflectcube.cpp
	scenenormalmap.cpp
	sceneprojtex.cpp
	scenerefractcube.cpp
	scenesamplerobj.cpp 
	scenediffibl.cpp scenediffibl.h
	sceneparallax.cpp sceneparallax.h)

add_executable( ${target} ${chapter05_SOURCES} )
target_compile_definitions(${target}
		PRIVATE
		GLFW_INCLUDE_NONE
		)

target_link_libraries( ${target}
		PRIVATE
		ingredients
		glfw
		${OPENGL_gl_LIBRARY}
		)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
