set(
	VANILLA_SOURCE_FILES
	"Game.cpp"
	"State.cpp"
	"Gfx/Bitmaps.cpp"
	"Gfx/Draw.cpp"
	"Gfx/Text.cpp"
	"Gfx/Shaders.cpp"
	"Gfx/Path.cpp"
	"Gfx/OpenGL.cpp"
	"Gfx/Transform.cpp"
	"Gfx/Gfx.cpp"
	"Gfx/Renderer.cpp"
	"Resources/ResourceManager.cpp"
	"Resources/Bitmap.cpp"
	"Resources/Font.cpp"
	"Physics/PhysicsBody.cpp"
	"Scene/Scene.cpp"
	"Scene/Entity.cpp"
	"Scene/Camera.cpp"
	"Scene/Component.cpp"
	"Util/Lodepng.cpp"
)


include_directories("/opt/vc/include")

find_library(
	libegl_location
	NAMES libbrcmEGL.so libEGL.so
	PATHS /opt/vc/lib
)
find_library(
	libgles_location
	NAMES libbrcmGLESv2.so libGLESv2.so
	PATHS /opt/vc/lib
)

# print locations
message(${libgles_location})
message(${libegl_location})

# add libraries
add_library(libegl SHARED IMPORTED)
add_library(libgles SHARED IMPORTED)

# set library locations
set_property(
	TARGET libegl
	PROPERTY IMPORTED_LOCATION
	${libegl_location}
)
set_property(
	TARGET libgles
	PROPERTY IMPORTED_LOCATION
	${libgles_location}
)

# make library
add_library(libLedlib2d SHARED ${VANILLA_SOURCE_FILES})
set_property(TARGET libLedlib2d PROPERTY POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(
	libLedlib2d
	libLedlib
	libgles
	libegl
)
