# Project
project("matrix-raspi-3d")

# Urho3D stuff
# Set CMake minimum version and CMake policy required by Urho3D-CMake-common module
if (WIN32)
	cmake_minimum_required (VERSION 3.2.3)      # Going forward all platforms will use this as minimum version
else ()
	cmake_minimum_required (VERSION 2.8.6)
endif ()
if (COMMAND cmake_policy)
	cmake_policy (SET CMP0003 NEW)
	if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
		# INTERFACE_LINK_LIBRARIES defines the link interface
		cmake_policy (SET CMP0022 NEW)
	endif ()
	if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
		# Disallow use of the LOCATION target property - so we set to OLD as we still need it
		cmake_policy (SET CMP0026 OLD)
		# MACOSX_RPATH is enabled by default
		cmake_policy (SET CMP0042 NEW)
	endif ()
endif ()
# Set Urho3D modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
set (URHO3D_HOME "${CMAKE_SOURCE_DIR}/../Urho3D")

set(LEDLIB_ROOT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(LEDLIB_CONFIG_PATH "${CMAKE_CURRENT_BINARY_DIR}/Ledlib")
set(LEDLIB_LAUNCHER_ARGS_PATH "${CMAKE_CURRENT_BINARY_DIR}/Launcher/args.txt")
set(LEDLIB_WEB_PATH "${CMAKE_SOURCE_DIR}/web")
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

add_definitions(
	-DLEDLIB_CONFIG_PATH="${LEDLIB_CONFIG_PATH}"
	-DLEDLIB_LAUNCHER_ARGS_PATH="${LEDLIB_LAUNCHER_ARGS_PATH}"
	-DLEDLIB_APPS_PATH="${CMAKE_CURRENT_BINARY_DIR}/Apps"
	-DLEDLIB_RESOURCES_PATH="${CMAKE_SOURCE_DIR}/Resources"
	-DLEDLIB_WEB_PATH="${LEDLIB_WEB_PATH}"
)

include_directories(
	"."
	"${URHO3D_HOME}/build/include/Urho3D/ThirdParty"
	"${URHO3D_HOME}/build/include/Urho3D/ThirdParty/SDL"
)

# Launcher
add_subdirectory("Launcher")

# Ledlib
add_subdirectory("Ledlib")

# Ledlib Urho3D
add_subdirectory("Ledlib3d")

# Ledlib Vanilla
add_subdirectory("Ledlib2d")

function(post_app app_name app_source_dir app_binary_dir)
	execute_process(COMMAND bash -c "touch ${app_binary_dir}/ledlib.app.cfg")
	# execute_process(COMMAND bash -c "chmod 666 \"${app_binary_dir}/ledlib.app.cfg\"")
	execute_process(COMMAND bash -c "chmod -R 777 \"${CMAKE_SOURCE_DIR}\"")
endfunction(post_app)

function(add_app app_name)
	add_subdirectory("Apps/${app_name}")
endfunction(add_app)

function(urho3d_setup app_name)
	# Define target name
	set(TARGET_NAME ${app_name})
	set(DEST_RUNTIME_DIR "${CMAKE_CURRENT_BINARY_DIR}/Apps/${app_name}")
	# Include Urho3D Cmake common module
	include(UrhoCommon)
	# Define source files
	define_source_files()
	# Setup target with resource copying
	setup_main_executable()
endfunction(urho3d_setup)


# add apps
add_app("Welcome")
add_app("Painter")
add_app("Gallery")
add_app("Urho3D-Test")
add_app("Shader-Playground")
add_app("Flappy-Bird")
add_app("Tetris")
add_app("Pong")
add_app("Asteroids")
add_app("Clock-Digital")
add_app("Snake")

#add_app("Ledlib-Test")
#add_app("Shader-Test")
#add_app("Ledlib2d-Test")
#add_app("Earth")



