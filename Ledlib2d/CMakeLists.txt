set(
	LEDLIB2D_SOURCE_FILES
	"App.cpp"
	"Game.cpp"
	"State.cpp"
	"Scene.cpp"
	"Entity.cpp"
	"Camera.cpp"
	"Component.cpp"
	"WeakPointerList.cpp"
	"Gfx/Bitmaps.cpp"
	"Gfx/Draw.cpp"
	"Gfx/Text.cpp"
	"Gfx/Path.cpp"
	"Gfx/OpenGL.cpp"
	"Gfx/Transform.cpp"
	"Gfx/Gfx.cpp"
	"Gfx/Renderer.cpp"
	"Gfx/Shader.cpp"
	"Gfx/Shaders.cpp"
	"Gfx/ShaderBox.cpp"
	"Resources/ResourceManager.cpp"
	"Resources/Bitmap.cpp"
	"Resources/Font.cpp"
	"Physics/PhysicsBody.cpp"
	"Physics/CollisionManager.cpp"
	"Physics/CollisionData.cpp"
	"Physics/Collider.cpp"
	"Physics/BoxCollider.cpp"
	"Physics/PolygonCollider.cpp"
	"Physics/CircleCollider.cpp"
	"Physics/PointCollider.cpp"
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
add_library(libLedlib2d SHARED ${LEDLIB2D_SOURCE_FILES})
set_property(TARGET libLedlib2d PROPERTY POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(
	libLedlib2d
	libLedlib
	libgles
	libegl
)
