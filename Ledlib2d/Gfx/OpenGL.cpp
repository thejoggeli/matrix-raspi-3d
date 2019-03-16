#include "OpenGL.h"
#include "Ledlib/Log.h"
#include "Ledlib/Config.h"

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <vector>

namespace Ledlib {

// EGL Variables
static EGLDisplay _display;
static int _major, _minor;
static EGLint _numConfigs;
static EGLConfig _config;
static EGLSurface _surface;
static EGLContext _context;
static GLint _viewport[4];
static std::vector<EGLint> _configAttribs;
static std::vector<EGLint> _pbufferAttribs;
static std::vector<EGLint> _contextAttribs;

namespace Gfx {

RectangleShape rectangleShape;
PolygonShape polygonShape;
PointShape pointShape;

// init
bool InitOpenGL(int width, int height){

	_configAttribs = {
		EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_DEPTH_SIZE, 16,
		// Uncomment the following to enable MSAA
	//	EGL_SAMPLE_BUFFERS, 1, // <-- Must be set to 1 to enable multisampling!
	//	EGL_SAMPLES, 4, // <-- Number of samples
		// Uncomment the following to enable stencil buffer
		//EGL_STENCIL_SIZE, 1,
		EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	};
	// Width and height of the desired framebuffer
	_pbufferAttribs = {
		EGL_WIDTH, 1,
		EGL_HEIGHT, 1,
		EGL_NONE,
	};
	_contextAttribs = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	std::string apiVersion = Config::GetString("opengl_api");

	// api version
	if(apiVersion == "opengl_es"){
		eglBindAPI(EGL_OPENGL_ES_API);
		Log(LOG_INFO, "Gfx/OpenGL", "OpenGL API set to EGL_OPENGL_ES_API");
	} else if(apiVersion == "opengl"){
		eglBindAPI(EGL_OPENGL_API);
		Log(LOG_INFO, "Gfx/OpenGL", "OpenGL API set to EGL_OPENGL_API");
	} else {
		Log(LOG_ERROR, "Gfx/OpenGL", "Unknown OpenGL API. set opengl_api in matlib.usr.cfg");
		apiVersion = "opengl";
		eglBindAPI(EGL_OPENGL_API);
		Log(LOG_INFO, "Gfx/OpenGL", "OpenGL API set to EGL_OPENGL_API (fallback)");
	}
	if(PrintOpenGLError("eglBindAPI")) return false;

	// use framebuffer?
	bool useFramebuffer = false;
	if(false/*apiVersion == "opengl"*/){
		useFramebuffer = true;
	} else {
		_pbufferAttribs[1] = width;
		_pbufferAttribs[3] = height;
	}

	// multisampling
	bool multisampling = Config::GetBool("opengl_multisampling_enabled", false);
	if(multisampling){
		int samples = Config::GetInt("opengl_multisampling_samples", 4);
		_configAttribs.push_back(EGL_SAMPLE_BUFFERS);
		_configAttribs.push_back(1);
		_configAttribs.push_back(EGL_SAMPLES);
		_configAttribs.push_back(samples);
		Log(LOG_INFO, "Gfx/OpenGL", iLog << "Multisampling x" << samples << " enabled");
	}
	_configAttribs.push_back(EGL_NONE);

	// display
	_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(PrintOpenGLError("eglGetDisplay")) return false;

	// initialize
	if(eglInitialize(_display, &_major, &_minor) == EGL_FALSE){
		PrintOpenGLError("eglInitialize");
		return false;
	}
	if(PrintOpenGLError("eglInitialize")) return false;
	Log(LOG_INFO, "Gfx/OpenGL", iLog << "Initialized EGL version " << _major << "." << _minor);

	// choose config
	if(!eglChooseConfig(_display, _configAttribs.data(), &_config, 1, &_numConfigs)){
		PrintOpenGLError("eglChooseConfig");
		return false;
	}
	if(PrintOpenGLError("eglChooseConfig")) return false;

	// surface
	_surface = eglCreatePbufferSurface(_display, _config, _pbufferAttribs.data());
	if(_surface == EGL_NO_SURFACE){
		PrintOpenGLError("eglCreatePbufferSurface");
		return false;
	}
	if(PrintOpenGLError("eglCreatePbufferSurface")) return false;

	_context = eglCreateContext(_display, _config, EGL_NO_CONTEXT, _contextAttribs.data());
	if(_context == EGL_NO_CONTEXT){
		PrintOpenGLError("eglCreateContext");
		return false;
	}
	if(PrintOpenGLError("eglCreateContext")) return false;

	eglMakeCurrent(_display, _surface, _surface, _context);
	if(PrintOpenGLError("eglMakeCurrent")) return false;

	// fbo
	if(useFramebuffer){
		GLuint fboId = 0;
		glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);

		GLuint renderBuffer;
		glGenRenderbuffers(1, &renderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderBuffer);

		GLuint depthRenderbuffer;
		glGenRenderbuffers(1, &depthRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status == GL_FRAMEBUFFER_COMPLETE){
			Log(LOG_DEBUG, "Gfx/OpenGL", "FBO creation success");
		} else {
			Log(LOG_DEBUG, "Gfx/OpenGL", iLog << "FBO creation failed: " << status);
		}
	}

	// rectangle vertices buffer
	glGenBuffers(1, &rectangleShape.vertices_vbo);
	if(PrintOpenGLError("glGenBuffers")) return false;
	glBindBuffer(GL_ARRAY_BUFFER, rectangleShape.vertices_vbo);
	if(PrintOpenGLError("glBindBuffer")) return false;
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleShape.vertices), rectangleShape.vertices, GL_STATIC_DRAW);
	if(PrintOpenGLError("glBufferData")) return false;

	// rectangle uvs buffer
	glGenBuffers(1, &rectangleShape.uvs_vbo);
	if(PrintOpenGLError("glGenBuffers")) return false;
	glBindBuffer(GL_ARRAY_BUFFER, rectangleShape.uvs_vbo);
	if(PrintOpenGLError("glBindBuffer")) return false;
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleShape.uvs), rectangleShape.uvs, GL_STATIC_DRAW);
	if(PrintOpenGLError("glBufferData")) return false;

	// polygon buffer
	glGenBuffers(1, &polygonShape.vbo);
	if(PrintOpenGLError("glGenBuffers")) return false;
	glBindBuffer(GL_ARRAY_BUFFER, polygonShape.vbo);
	if(PrintOpenGLError("glBindBuffer")) return false;
	glBufferData(GL_ARRAY_BUFFER, sizeof(polygonShape.vertices), polygonShape.vertices, GL_DYNAMIC_DRAW);
	if(PrintOpenGLError("glBufferData")) return false;

	// point buffer
	glGenBuffers(1, &pointShape.vbo);
	if(PrintOpenGLError("glGenBuffers")) return false;
	glBindBuffer(GL_ARRAY_BUFFER, pointShape.vbo);
	if(PrintOpenGLError("glBindBuffer")) return false;
	glBufferData(GL_ARRAY_BUFFER, sizeof(pointShape.vertices), pointShape.vertices, GL_DYNAMIC_DRAW);
	if(PrintOpenGLError("glBufferData")) return false;

	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// enable depth test
//	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
//	glDepthMask(GL_TRUE);
//	glEnable(GL_CULL_FACE);

	// set viewport
	glViewport(0, 0, width, height);
	if(PrintOpenGLError("glViewport")) return false;

	// print viewport
	glGetIntegerv(GL_VIEWPORT, _viewport);
	Log(LOG_INFO, "Gfx/OpenGL", iLog <<"GL Viewport size: " << _viewport[2] << "x" << _viewport[3]);
	if(PrintOpenGLError("glGetIntegerv")) return false;

	return true;
}

bool GenerateTextureFromFramebuffer(unsigned int *t, unsigned int *f, unsigned int w, unsigned int h){
	glGenFramebuffers(1, f);
	glGenTextures(1, t);

	glBindFramebuffer(GL_FRAMEBUFFER, *f);

	glBindTexture(GL_TEXTURE_2D, *t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *t, 0);

	GLuint depthbuffer;
	glGenRenderbuffers(1, &depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status != GL_FRAMEBUFFER_COMPLETE){
		Log(LOG_ERROR, "OpenGL", iLog << "Framebuffer status: " << status);
		return false;
	}
	return true;
}

bool PrintOpenGLError(const std::string& context){

	int error = eglGetError();
	if(error != EGL_SUCCESS){
		Log(LOG_ERROR, "Gfx/OpenGL", iLog << context + ": " << GetEglErrorString(error));
		return true;
	}
	return false;
}

const std::string GetEglErrorString(int error){
	switch(error){
		case EGL_SUCCESS: return "The last function succeeded without error.";
		case EGL_NOT_INITIALIZED: return "EGL is not initialized, or could not be initialized, for the specified EGL display connection.";
		case EGL_BAD_ACCESS: return "EGL cannot access a requested resource (for example a context is bound in another thread).";
		case EGL_BAD_ALLOC: return "EGL failed to allocate resources for the requested operation.";
		case EGL_BAD_ATTRIBUTE: return "An unrecognized attribute or attribute value was passed in the attribute list.";
		case EGL_BAD_CONTEXT: return "An EGLContext argument does not name a valid EGL rendering context.";
		case EGL_BAD_CONFIG: return "An EGLConfig argument does not name a valid EGL frame buffer configuration.";
		case EGL_BAD_CURRENT_SURFACE: return "The current surface of the calling thread is a window, pixel buffer or pixmap that is no longer valid.";
		case EGL_BAD_DISPLAY: return "An EGLDisplay argument does not name a valid EGL display connection.";
		case EGL_BAD_SURFACE: return "An EGLSurface argument does not name a valid surface (window, pixel buffer or pixmap) configured for GL rendering.";
		case EGL_BAD_MATCH: return "Arguments are inconsistent (for example, a valid context requires buffers not supplied by a valid surface).";
		case EGL_BAD_PARAMETER: return "One or more argument values are invalid.";
		case EGL_BAD_NATIVE_PIXMAP: return "A NativePixmapType argument does not refer to a valid native pixmap.";
		case EGL_BAD_NATIVE_WINDOW: return "A NativeWindowType argument does not refer to a valid native window.";
		case EGL_CONTEXT_LOST: return "A power management event has occurred. The application must destroy all contexts and reinitialise OpenGL ES state and objects to continue rendering.";
		default: break;
	}
	return "Unknown error!";
}

}
}
