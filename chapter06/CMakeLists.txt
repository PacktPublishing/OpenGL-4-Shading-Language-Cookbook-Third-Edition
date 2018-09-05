set(target chapter06)
set( chapter06_SOURCES
	main.cpp
	sceneblur.cpp sceneblur.h
	scenedeferred.cpp scenedeferred.h
	sceneedge.cpp sceneedge.h
	scenegamma.cpp scenegamma.h
	scenemsaa.cpp scenemsaa.h
	scenetonemap.cpp scenetonemap.h
	scenehdrbloom.cpp scenehdrbloom.h 
		sceneoit.cpp sceneoit.h
		scenessao.cpp scenessao.h)

add_executable( ${target} ${chapter06_SOURCES} )

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

set_target_properties(${target} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/chapter05")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
