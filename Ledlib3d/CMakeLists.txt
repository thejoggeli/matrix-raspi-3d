set(
	LEDLIB3D_SOURCE_FILES
	"Ledlib3dApplication.cpp"
)

# make library
add_library(libLedlib3d SHARED ${LEDLIB3D_SOURCE_FILES})
set_property(TARGET libLedlib3d PROPERTY POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(
	libLedlib3d
	libLedlib
)
