set(
	URHO3D_GLUE_SOURCE_FILES
	"Urho3DApplication.cpp"
)

# make library
add_library(Urho3DGlue SHARED ${URHO3D_GLUE_SOURCE_FILES})
set_property(TARGET Urho3DGlue PROPERTY POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(
	Urho3DGlue
	libLedlib
)

