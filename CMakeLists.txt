cmake_minimum_required(VERSION 2.8)

set(LEDLIB_ROOT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(LEDLIB_LAUNCHER_ARGS_PATH "${LEDLIB_ROOT_DIR}/launcher/args.txt")

add_definitions(
	-DLEDLIB_CONFIG_PATH="${CMAKE_CURRENT_BINARY_DIR}/Ledlib"
	-DLEDLIB_LAUNCHER_ARGS_PATH="${LEDLIB_LAUNCHER_ARGS_PATH}"
)

add_subdirectory("Ledlib")

# matlib
add_library(libledlib SHARED IMPORTED)
set_property(
	TARGET libledlib
	PROPERTY IMPORTED_LOCATION
	${CMAKE_CURRENT_BINARY_DIR}/Ledlib/libLedlib.so
)

set(LEDLIB_APP_LIBS libledlib)
include_directories(".")

function(post_app app_name app_source_dir app_binary_dir)
	execute_process(COMMAND bash -c "touch ${app_binary_dir}/ledlib.app.cfg")
	execute_process(COMMAND bash -c "chmod 666 \"${app_binary_dir}/ledlib.app.cfg\"")
endfunction(post_app)

function(add_app app_name)
	add_subdirectory("Apps/${app_name}")
endfunction(add_app)

add_app("Urho3D-Test")
add_app("Ledlib-Test")
