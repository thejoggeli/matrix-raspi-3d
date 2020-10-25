project(${app_name})
add_executable(
	${app_name}
	"main.cpp"
    "MyApp.cpp"
    "anim/Anim.cpp"
    "anim/AnimSW.cpp"
)
include_directories(".")
target_link_libraries(
	${app_name}
	libLedlib2d
)
post_app(${app_name} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
