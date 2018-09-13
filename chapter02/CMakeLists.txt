set(target chapter02)

set( chapter02_SOURCES main.cpp sceneseparable.cpp
		scenebasic_uniformblock.cpp
		scenebasic_attrib.cpp
		scenebasic_uniform.cpp )

add_executable( ${target} ${chapter02_SOURCES} )

target_compile_definitions(${target}
		PRIVATE
		${DEFAULT_COMPILE_DEFINITIONS}
		GLFW_INCLUDE_NONE
		)

target_link_libraries( ${target}
        PRIVATE
        ingredients
		glfw
        ${OPENGL_gl_LIBRARY}
		)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
