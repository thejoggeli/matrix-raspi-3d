project(${app_name})
add_executable(
	${app_name}
	"main.cpp"
	"ShaderPlayground.cpp"
	"ShaderState.cpp"
	"Shaders/Basic.cpp"
	"Shaders/Mandelbrot.cpp"
)
include_directories(".")
target_link_libraries(
	${app_name}
	libLedlib2d
)
post_app(${app_name} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
