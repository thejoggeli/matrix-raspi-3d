#include "X11Display.h"
#include "../Ledlib.h"
#include "../Config.h"
#include "../Log.h"
#include <X11/Xlib.h>
#include <unistd.h>

static Display *display;
static Window window;
static XEvent event;
static int screen;
static Atom wmDeleteMessage;
static GC gc;

namespace Ledlib {

X11Display::X11Display(){}


bool X11Display::Init() {

	// read config file
	int cols = Config::GetInt("mat_cols");
	int rows = Config::GetInt("mat_rows");
	int chain = Config::GetInt("mat_chain");
	int parallel = Config::GetInt("mat_parallel");
	int orientation = Config::GetInt("mat_orientation");
	int d1 = rows*parallel;
	int d2 = cols*chain;
	int dmax = (d1 > d2) ? d1 : d2;
	int dmin = (d1 < d2) ? d1 : d2;
	if(orientation == 0){
		SetDimensions(dmax, dmin);
	} else {
		SetDimensions(dmin, dmax);
	}
	SetPixelOffset(Config::GetInt("display_sim_pixel_offset"));
	SetPixelScale(Config::GetInt("display_sim_pixel_scale"));

	Log(LOG_INFO, "X11Display", iLog << "Dimensions: " << width << "x" << height);
	Log(LOG_INFO, "X11Display", iLog << "Pixel format: scale=" << pixelScale << ", offset=" << pixelOffset);

/*	SetDimensions(480, 360);
	SetPixelOffset(0);
	SetPixelScale(1); */

	// create x11 window
	display = XOpenDisplay(nullptr);
	if (display == nullptr) {
		Log(LOG_ERROR, "X11Display", "Cannot open display");
		return false;
	}
	screen = DefaultScreen(display);
	window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, windowWidth, windowHeight, 1, WhitePixel(display, screen), BlackPixel(display, screen));
	XSelectInput(display, window, ExposureMask | KeyPressMask);
	XMapWindow(display, window);
	wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(display, window, &wmDeleteMessage, 1);
	XSync(display, False);
	gc = DefaultGC(display, screen);
	XStoreName(display, window, "Matrix Simulator");

	return true;
}
void X11Display::SetPixelOffset(int pixelOffset){
	this->pixelOffset = pixelOffset;
	this->pixelOffset2 = pixelOffset*2;
}
void X11Display::SetPixelScale(int pixelScale){
	this->pixelScale = pixelScale;
	windowWidth = width * pixelScale;
	windowHeight = height * pixelScale;
}
void X11Display::Sync() {
	while(XPending(display)){
		XNextEvent(display, &event);
		switch  (event.type){
		case ClientMessage:
			Log(LOG_DEBUG, "X11Display", "ClientMessage received");
			if(event.xclient.data.l[0] == wmDeleteMessage){
				Log(LOG_DEBUG, "X11Display", "Close window requested");
				LedMatrixLibrary::RequestExit();
			}
			break;
		}
	}
	(*pixels)[0];

	unsigned int w = static_cast<unsigned int>(pixelScale - pixelOffset2);
	unsigned int h = static_cast<unsigned int>(pixelScale - pixelOffset2);
	unsigned long i = 0;
	unsigned long pitch = static_cast<unsigned long>(this->pitch);
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			XSetForeground(display, gc,
				(static_cast<uint32_t>((*pixels)[i]) << 16u) +
				(static_cast<uint32_t>((*pixels)[i+1]) << 8u) +
				(static_cast<uint32_t>((*pixels)[i+2]))
			);
			XFillRectangle(
				display, window, gc,
				x * pixelScale + pixelOffset, y * pixelScale + pixelOffset, w, h
			);
			i += pitch;
		}
	}
	usleep(10000);
}

void X11Display::Shutdown(){
	XCloseDisplay(display);
}

}
