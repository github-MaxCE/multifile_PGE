#pragma once
#ifndef OLCRENDERER_OGL33_H
#define OLCRENDERER_OGL33_H
#include "olcV2D.h"
#include "olcRenderer.h"

#pragma region renderer_ogl33
// O------------------------------------------------------------------------------O
// | START RENDERER: OpenGL 3.3 (3.0 es) (sh-sh-sh-shaders....)                   |
// O------------------------------------------------------------------------------O
#if !defined(OLC_GFX_OPENGL33)

#if defined(OLC_PLATFORM_WINAPI)
#include <dwmapi.h>
#include <gl/GL.h>
#if !defined(__MINGW32__)
#pragma comment(lib, "Dwmapi.lib")
#endif		
typedef void __stdcall locSwapInterval_t(GLsizei n);
typedef HDC glDeviceContext_t;
typedef HGLRC glRenderContext_t;
#define CALLSTYLE __stdcall
#define OGL_LOAD(t, n) (t*)wglGetProcAddress(#n)
#endif

#if defined(__linux__) || defined(__FreeBSD__)
#include <GL/gl.h>
#endif

#if defined(OLC_PLATFORM_X11)
namespace X11
{
#include <GL/glx.h>
}
typedef int(locSwapInterval_t)(X11::Display* dpy, X11::GLXDrawable drawable, int interval);
typedef X11::GLXContext glDeviceContext_t;
typedef X11::GLXContext glRenderContext_t;
#define CALLSTYLE 
#define OGL_LOAD(t, n) (t*)glXGetProcAddress((unsigned char*)#n);
#endif

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#if defined(OLC_PLATFORM_EMSCRIPTEN)
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>
#include <emscripten/emscripten.h>
#define CALLSTYLE
typedef EGLBoolean(locSwapInterval_t)(EGLDisplay display, EGLint interval);
#define GL_CLAMP GL_CLAMP_TO_EDGE
#define OGL_LOAD(t, n) n;
#endif

namespace olc
{
	typedef char GLchar;
	typedef ptrdiff_t GLsizeiptr;
	typedef GLuint CALLSTYLE locCreateShader_t(GLenum type);
	typedef GLuint CALLSTYLE locCreateProgram_t(void);
	typedef void CALLSTYLE locDeleteShader_t(GLuint shader);
#if defined(OLC_PLATFORM_EMSCRIPTEN)
	typedef void CALLSTYLE locShaderSource_t(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
#else
	typedef void CALLSTYLE locShaderSource_t(GLuint shader, GLsizei count, const GLchar** string, const GLint* length);
#endif
	typedef void CALLSTYLE locCompileShader_t(GLuint shader);
	typedef void CALLSTYLE locLinkProgram_t(GLuint program);
	typedef void CALLSTYLE locDeleteProgram_t(GLuint program);
	typedef void CALLSTYLE locAttachShader_t(GLuint program, GLuint shader);
	typedef void CALLSTYLE locBindBuffer_t(GLenum target, GLuint buffer);
	typedef void CALLSTYLE locBufferData_t(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	typedef void CALLSTYLE locGenBuffers_t(GLsizei n, GLuint* buffers);
	typedef void CALLSTYLE locVertexAttribPointer_t(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	typedef void CALLSTYLE locEnableVertexAttribArray_t(GLuint index);
	typedef void CALLSTYLE locUseProgram_t(GLuint program);
	typedef void CALLSTYLE locBindVertexArray_t(GLuint array);
	typedef void CALLSTYLE locGenVertexArrays_t(GLsizei n, GLuint* arrays);
	typedef void CALLSTYLE locGetShaderInfoLog_t(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);

	constexpr size_t OLC_MAX_VERTS = 128;

	class Renderer_OGL33 : public olc::Renderer
	{
	private:
#if defined(OLC_PLATFORM_EMSCRIPTEN)
		EGLDisplay olc_Display;
		EGLConfig olc_Config;
		EGLContext olc_Context;
		EGLSurface olc_Surface;
#endif

#if defined(OLC_PLATFORM_GLUT)
		bool mFullScreen = false;
#else
#if !defined(OLC_PLATFORM_EMSCRIPTEN)
		glDeviceContext_t glDeviceContext = 0;
		glRenderContext_t glRenderContext = 0;
#endif
#endif
		bool bSync = false;
		olc::DecalMode nDecalMode = olc::DecalMode(-1); // Thanks Gusgo & Bispoo
#if defined(OLC_PLATFORM_X11)
		X11::Display* olc_Display = nullptr;
		X11::Window* olc_Window = nullptr;
		X11::XVisualInfo* olc_VisualInfo = nullptr;
#endif

	private:
		locCreateShader_t* locCreateShader = nullptr;
		locShaderSource_t* locShaderSource = nullptr;
		locCompileShader_t* locCompileShader = nullptr;
		locDeleteShader_t* locDeleteShader = nullptr;
		locCreateProgram_t* locCreateProgram = nullptr;
		locDeleteProgram_t* locDeleteProgram = nullptr;
		locLinkProgram_t* locLinkProgram = nullptr;
		locAttachShader_t* locAttachShader = nullptr;
		locBindBuffer_t* locBindBuffer = nullptr;
		locBufferData_t* locBufferData = nullptr;
		locGenBuffers_t* locGenBuffers = nullptr;
		locVertexAttribPointer_t* locVertexAttribPointer = nullptr;
		locEnableVertexAttribArray_t* locEnableVertexAttribArray = nullptr;
		locUseProgram_t* locUseProgram = nullptr;
		locBindVertexArray_t* locBindVertexArray = nullptr;
		locGenVertexArrays_t* locGenVertexArrays = nullptr;
		locSwapInterval_t* locSwapInterval = nullptr;
		locGetShaderInfoLog_t* locGetShaderInfoLog = nullptr;

		uint32_t m_nFS = 0;
		uint32_t m_nVS = 0;
		uint32_t m_nQuadShader = 0;
		uint32_t m_vbQuad = 0;
		uint32_t m_vaQuad = 0;

		struct locVertex
		{
			float pos[3];
			olc::vf2d tex;
			olc::Pixel col;
		};

		locVertex pVertexMem[OLC_MAX_VERTS];

		olc::Renderable rendBlankQuad;

	public:
		void PrepareDevice() override;
		olc::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) override;
		olc::rcode DestroyDevice() override;
		void DisplayFrame() override;
		void PrepareDrawing() override;
		void SetDecalMode(const olc::DecalMode& mode) override;
		void DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint) override;
		void DrawDecal(const olc::DecalInstance& decal) override;
		uint32_t CreateTexture(const uint32_t width, const uint32_t height, const bool filtered, const bool clamp) override;
		uint32_t DeleteTexture(const uint32_t id) override;
		void UpdateTexture(uint32_t id, olc::Sprite* spr) override;
		void ReadTexture(uint32_t id, olc::Sprite* spr) override;
		void ApplyTexture(uint32_t id) override;
		void ClearBuffer(olc::Pixel p, bool bDepth) override;
		void UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size) override;
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END RENDERER: OpenGL 3.3 (3.0 es) (sh-sh-sh-shaders....)                     |
// O------------------------------------------------------------------------------O
#pragma endregion
#endif //OLCRENDERER_OGL33_H