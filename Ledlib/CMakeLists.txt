set(
	MATLIB_SOURCE_FILES
	"Ledlib.cpp"
	"LedlibEventHandler.cpp"
	"Time.cpp"
	"Log.cpp"
	"Config.cpp"
	"Math/Numbers.cpp"
	"Math/Vector2f.cpp"
	"Math/Vector2i.cpp"
	"Math/Rect4f.cpp"
	"Math/Rect4i.cpp"
	"Util/Strings.cpp"
	"Util/Timer.cpp"
	"Util/ClientJoiner.cpp"
	"Util/ClientJoinerSlot.cpp"
	"Util/ClientJoinerListener.cpp"
	"Util/ColorRgb.cpp"
	"Util/ColorHsl.cpp"
	"Remote/Client.cpp"
	"Remote/ClientManager.cpp"
	"Remote/Server.cpp"
	"Remote/SeasocksServer.cpp"
	"Remote/ServerManager.cpp"
	"Remote/ServerMessage.cpp"
	"Remote/Keys.cpp"
	"Events/Event.cpp"
	"Events/EventManager.cpp"
	"Display/Display.cpp"
	"Display/DisplayManager.cpp"
	"Display/X11Display.cpp"
	"Display/MatrixDisplay.cpp"
	"Display/DummyDisplay.cpp"
	"Sfx/RemoteSfx.cpp"
	"Util/Lodepng.cpp"
)

find_library(
	libseasocks_location
	NAMES libseasocks.so
	PATHS /usr/local/lib/ /usr/lib
)
find_library(
	librgbmatrix_location
	NAMES librgbmatrix.so.1
	PATHS /usr/local/lib /usr/lib
)
find_library(
	libx11_location
	NAMES libX11.so
	PATHS /usr/lib/arm-linux-gnueabihf
)

# print locations
message(${libseasocks_location})
message(${librgbmatrix_location})
message(${libx11_location})

# add libraries
add_library(libseasocks SHARED IMPORTED)
add_library(librgbmatrix SHARED IMPORTED)
add_library(libx11 SHARED IMPORTED)

# set library locations
set_property(
	TARGET librgbmatrix
	PROPERTY IMPORTED_LOCATION
	${libseasocks_location}
)
set_property(
	TARGET libseasocks
	PROPERTY IMPORTED_LOCATION
	${librgbmatrix_location}
)
set_property(
	TARGET libx11
	PROPERTY IMPORTED_LOCATION
	${libx11_location}
)

# make library
add_library(libLedlib SHARED ${MATLIB_SOURCE_FILES})
set_property(TARGET libLedlib PROPERTY POSITION_INDEPENDENT_CODE TRUE)

target_link_libraries(
	libLedlib
	libseasocks
	librgbmatrix
	libx11
)

execute_process(COMMAND bash -c "cp -rf ${CMAKE_CURRENT_SOURCE_DIR}/Config/ledlib.def.cfg ${CMAKE_CURRENT_BINARY_DIR}/ledlib.def.cfg")
execute_process(COMMAND bash -c "cp -n ${CMAKE_CURRENT_SOURCE_DIR}/Config/ledlib.usr.cfg ${CMAKE_CURRENT_BINARY_DIR}/ledlib.usr.cfg")
execute_process(COMMAND bash -c "chmod 666 \"${CMAKE_CURRENT_BINARY_DIR}/ledlib.def.cfg\"")
execute_process(COMMAND bash -c "chmod 666 \"${CMAKE_CURRENT_BINARY_DIR}/ledlib.usr.cfg\"")
