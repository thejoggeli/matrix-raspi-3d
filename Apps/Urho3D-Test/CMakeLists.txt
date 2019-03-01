project(${app_name})
include_directories(".")
urho3d_setup(${app_name})
target_link_libraries(
	${app_name}
	libUrho3DGlue
)
post_app(${app_name} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
