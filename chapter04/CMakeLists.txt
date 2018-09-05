set(target chapter04)
set( chapter04_SOURCES
	scenemultilight.cpp scenemultilight.h
	sceneperfragment.cpp sceneperfragment.h
	main.cpp
	scenefog.cpp scenefog.h
	scenedirectional.cpp scenedirectional.h
	scenetoon.cpp scenetoon.h
	scenespot.cpp scenespot.h
	scenepbr.cpp scenepbr.h )

add_executable( ${target} ${chapter04_SOURCES} )

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
