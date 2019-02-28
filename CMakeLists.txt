cmake_minimum_required(VERSION 2.8)

project("matrix-raspi-3d")

set(LEDLIB_ROOT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(LEDLIB_CONFIG_PATH "${CMAKE_CURRENT_BINARY_DIR}/Ledlib")
set(LEDLIB_LAUNCHER_ARGS_PATH "${LEDLIB_ROOT_DIR}/launcher/args.txt")
set(LEDLIB_DEFAULT_WEB_PATH "${LEDLIB_ROOT_DIR}/web")

add_definitions(
	-DLEDLIB_CONFIG_PATH="${LEDLIB_CONFIG_PATH}"
	-DLEDLIB_LAUNCHER_ARGS_PATH="${LEDLIB_LAUNCHER_ARGS_PATH}"
	-DLEDLIB_DEFAULT_WEB_PATH="${LEDLIB_DEFAULT_WEB_PATH}"
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
