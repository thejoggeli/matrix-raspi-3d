add_executable(
	${app_name}
	"main.cpp"
)
include_directories(".")
target_link_libraries(
	${app_name}
	${LEDLIB_APP_LIBS}
)
post_app(${app_name} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
