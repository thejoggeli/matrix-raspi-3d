set(
	URHO3D_GLUE_SOURCE_FILES
	"App.cpp"
	"AppManager.cpp"
	"State.cpp"
	"StateManager.cpp"
	"Gfx.cpp"
)

# make library
add_library(libLedlib3d SHARED ${URHO3D_GLUE_SOURCE_FILES})
set_property(TARGET libLedlib3d PROPERTY POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(
	libLedlib3d
	libLedlib
)

