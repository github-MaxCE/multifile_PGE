module;

export module olc.Renderer;

import <stdint.h>;
import std.core;
import olc.xtra;
import olc.V2D;
import olc.Pixel;
import olc.Sprite;
import olc.PGE;

export namespace olc
{
	class Renderer
	{
	public:
		virtual ~Renderer() = default;
		virtual void       PrepareDevice() = 0;
		virtual olc::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) = 0;
		virtual olc::rcode DestroyDevice() = 0;
		virtual void       DisplayFrame() = 0;
		virtual void       PrepareDrawing() = 0;
		virtual void	   SetDecalMode(const olc::DecalMode& mode) = 0;
		virtual void       DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint) = 0;
		virtual void       DrawDecal(const olc::DecalInstance& decal) = 0;
		virtual uint32_t   CreateTexture(const uint32_t width, const uint32_t height, const bool filtered = false, const bool clamp = true) = 0;
		virtual void       UpdateTexture(uint32_t id, olc::Sprite* spr) = 0;
		virtual void       ReadTexture(uint32_t id, olc::Sprite* spr) = 0;
		virtual uint32_t   DeleteTexture(const uint32_t id) = 0;
		virtual void       ApplyTexture(uint32_t id) = 0;
		virtual void       UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size) = 0;
		virtual void       ClearBuffer(olc::Pixel p, bool bDepth) = 0;
		static olc::PixelGameEngine* ptrPGE;
	};

	olc::PixelGameEngine* olc::Renderer::ptrPGE = nullptr;
}

// O------------------------------------------------------------------------------O
// | olcPixelGameEngine Renderers - the draw-y bits                               |
// O------------------------------------------------------------------------------O
#pragma region renderers
#pragma region ogl10
// O------------------------------------------------------------------------------O
// | START RENDERER: OpenGL 1.0 (the original, the best...)                       |
// O------------------------------------------------------------------------------O
#if defined(OLC_GFX_OPENGL10)


#if defined(OLC_PLATFORM_WINAPI)
#include <dwmapi.h>
#include <GL/gl.h>
#if !defined(__MINGW32__)
#pragma comment(lib, "Dwmapi.lib")
#endif
typedef BOOL(WINAPI wglSwapInterval_t) (int interval);
static wglSwapInterval_t* wglSwapInterval = nullptr;
typedef HDC glDeviceContext_t;
typedef HGLRC glRenderContext_t;
#endif


#if defined(__linux__) || defined(__FreeBSD__)
#include <GL/gl.h>
#endif


#if defined(OLC_PLATFORM_X11)
namespace X11
{
#include <GL/glx.h>
}
typedef int(glSwapInterval_t)(X11::Display* dpy, X11::GLXDrawable drawable, int interval);
static glSwapInterval_t* glSwapIntervalEXT;
typedef X11::GLXContext glDeviceContext_t;
typedef X11::GLXContext glRenderContext_t;
#endif


#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif


namespace olc
{
	export class Renderer_OGL10 : public olc::Renderer
	{
	private:
#if defined(OLC_PLATFORM_GLUT)
		bool mFullScreen = false;
#else
		glDeviceContext_t glDeviceContext = 0;
		glRenderContext_t glRenderContext = 0;
#endif


		bool bSync = false;
		olc::DecalMode nDecalMode = olc::DecalMode(-1); // Thanks Gusgo & Bispoo
		olc::DecalStructure nDecalStructure = olc::DecalStructure(-1);
#if defined(OLC_PLATFORM_X11)
		X11::Display* olc_Display = nullptr;
		X11::Window* olc_Window = nullptr;
		X11::XVisualInfo* olc_VisualInfo = nullptr;
#endif


	public:
		void PrepareDevice() override
		{
#if defined(OLC_PLATFORM_GLUT)
			//glutInit has to be called with main() arguments, make fake ones
			int argc = 0;
			char* argv[1] = { (char*)"" };
			glutInit(&argc, argv);
			glutInitWindowPosition(0, 0);
			glutInitWindowSize(512, 512);
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
			// Creates the window and the OpenGL context for it
			glutCreateWindow("OneLoneCoder.com - Pixel Game Engine");
			glEnable(GL_TEXTURE_2D); // Turn on texturing
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
		}


		olc::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) override
		{
#if defined(OLC_PLATFORM_WINAPI)
			// Create Device Context
			glDeviceContext = GetDC((HWND)(params[0]));
			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR), 1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				PFD_MAIN_PLANE, 0, 0, 0, 0
			};


			int pf = 0;
			if (!(pf = ChoosePixelFormat(glDeviceContext, &pfd))) return olc::FAIL;
			SetPixelFormat(glDeviceContext, pf, &pfd);


			if (!(glRenderContext = wglCreateContext(glDeviceContext))) return olc::FAIL;
			wglMakeCurrent(glDeviceContext, glRenderContext);


			// Remove Frame cap
			wglSwapInterval = (wglSwapInterval_t*)wglGetProcAddress("wglSwapIntervalEXT");
			if (wglSwapInterval && !bVSYNC) wglSwapInterval(0);
			bSync = bVSYNC;
#endif


#if defined(OLC_PLATFORM_X11)
			using namespace X11;
			// Linux has tighter coupling between OpenGL and X11, so we store
			// various "platform" handles in the renderer
			olc_Display = (X11::Display*)(params[0]);
			olc_Window = (X11::Window*)(params[1]);
			olc_VisualInfo = (X11::XVisualInfo*)(params[2]);


			glDeviceContext = glXCreateContext(olc_Display, olc_VisualInfo, nullptr, GL_TRUE);
			glXMakeCurrent(olc_Display, *olc_Window, glDeviceContext);


			XWindowAttributes gwa;
			XGetWindowAttributes(olc_Display, *olc_Window, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);


			glSwapIntervalEXT = nullptr;
			glSwapIntervalEXT = (glSwapInterval_t*)glXGetProcAddress((unsigned char*)"glXSwapIntervalEXT");


			if (glSwapIntervalEXT == nullptr && !bVSYNC)
			{
				printf("NOTE: Could not disable VSYNC, glXSwapIntervalEXT() was not found!\n");
				printf("      Don't worry though, things will still work, it's just the\n");
				printf("      frame rate will be capped to your monitors refresh rate - javidx9\n");
			}


			if (glSwapIntervalEXT != nullptr && !bVSYNC)
				glSwapIntervalEXT(olc_Display, *olc_Window, 0);
#endif		


#if defined(OLC_PLATFORM_GLUT)
			mFullScreen = bFullScreen;
			if (!bVSYNC)
			{
#if defined(__APPLE__)
				GLint sync = 0;
				CGLContextObj ctx = CGLGetCurrentContext();
				if (ctx) CGLSetParameter(ctx, kCGLCPSwapInterval, &sync);
#endif
			}
#else
			glEnable(GL_TEXTURE_2D); // Turn on texturing
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
			return olc::rcode::OK;
		}


		olc::rcode DestroyDevice() override
		{
#if defined(OLC_PLATFORM_WINAPI)
			wglDeleteContext(glRenderContext);
#endif


#if defined(OLC_PLATFORM_X11)
			glXMakeCurrent(olc_Display, None, NULL);
			glXDestroyContext(olc_Display, glDeviceContext);
#endif


#if defined(OLC_PLATFORM_GLUT)
			glutDestroyWindow(glutGetWindow());
#endif
			return olc::rcode::OK;
		}


		void DisplayFrame() override
		{
#if defined(OLC_PLATFORM_WINAPI)
			SwapBuffers(glDeviceContext);
			if (bSync) DwmFlush(); // Woooohooooooo!!!! SMOOOOOOOTH!
#endif	


#if defined(OLC_PLATFORM_X11)
			X11::glXSwapBuffers(olc_Display, *olc_Window);
#endif		


#if defined(OLC_PLATFORM_GLUT)
			glutSwapBuffers();
#endif
		}


		void PrepareDrawing() override
		{

			//ClearBuffer(olc::GREEN, true);
			glEnable(GL_BLEND);
			nDecalMode = DecalMode::NORMAL;
			nDecalStructure = DecalStructure::FAN;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}


		void SetDecalMode(const olc::DecalMode& mode)
		{
			if (mode != nDecalMode)
			{
				switch (mode)
				{
				case olc::DecalMode::NORMAL:
				case olc::DecalMode::MODEL3D:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case olc::DecalMode::ADDITIVE:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					break;
				case olc::DecalMode::MULTIPLICATIVE:
					glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
					break;
				case olc::DecalMode::STENCIL:
					glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
					break;
				case olc::DecalMode::ILLUMINATE:
					glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
					break;
				case olc::DecalMode::WIREFRAME:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				}


				nDecalMode = mode;
			}
		}


		void DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint) override
		{
			glBegin(GL_QUADS);
			glColor4ub(tint.r, tint.g, tint.b, tint.a);
			glTexCoord2f(0.0f * scale.x + offset.x, 1.0f * scale.y + offset.y);
			glVertex3f(-1.0f /*+ vSubPixelOffset.x*/, -1.0f /*+ vSubPixelOffset.y*/, 0.0f);
			glTexCoord2f(0.0f * scale.x + offset.x, 0.0f * scale.y + offset.y);
			glVertex3f(-1.0f /*+ vSubPixelOffset.x*/, 1.0f /*+ vSubPixelOffset.y*/, 0.0f);
			glTexCoord2f(1.0f * scale.x + offset.x, 0.0f * scale.y + offset.y);
			glVertex3f(1.0f /*+ vSubPixelOffset.x*/, 1.0f /*+ vSubPixelOffset.y*/, 0.0f);
			glTexCoord2f(1.0f * scale.x + offset.x, 1.0f * scale.y + offset.y);
			glVertex3f(1.0f /*+ vSubPixelOffset.x*/, -1.0f /*+ vSubPixelOffset.y*/, 0.0f);
			glEnd();
		}


		void DrawDecal(const olc::DecalInstance& decal) override
		{
			SetDecalMode(decal.mode);


			if (decal.decal == nullptr)
				glBindTexture(GL_TEXTURE_2D, 0);
			else
				glBindTexture(GL_TEXTURE_2D, decal.decal->id);

			if (nDecalMode == DecalMode::MODEL3D)
			{
#ifdef OLC_ENABLE_EXPERIMENTAL
				glMatrixMode(GL_PROJECTION); glPushMatrix();
				glMatrixMode(GL_MODELVIEW);  glPushMatrix();


				glEnable(GL_DEPTH_TEST);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glFrustum(-1.0f, 1.0f, -1.0f, 1.0f, 1, 1000);


#pragma comment (lib, "winmm.lib")


				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glTranslatef(0, -40, -200);
				glRotatef(float(clock()) * 0.1f, 1, 0, 0);
				glRotatef(float(clock()) * 0.1f * 2, 0, 1, 0);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


				glBegin(GL_TRIANGLES);



				// Render as 3D Spatial Entity
				for (uint32_t n = 0; n < decal.points; n++)
				{
					glColor4ub(decal.tint[n].r, decal.tint[n].g, decal.tint[n].b, decal.tint[n].a);
					glTexCoord2f(decal.uv[n].x, decal.uv[n].y);
					glVertex3f(decal.pos[n].x, decal.pos[n].y, decal.w[n]);
				}


				glEnd();


				glMatrixMode(GL_PROJECTION); glPopMatrix();
				glMatrixMode(GL_MODELVIEW);  glPopMatrix();
				glDisable(GL_DEPTH_TEST);
#endif
			}
			else
			{
				if (nDecalMode == DecalMode::WIREFRAME)
					glBegin(GL_LINE_LOOP);
				else
				{
					if (decal.structure == olc::DecalStructure::FAN)
						glBegin(GL_TRIANGLE_FAN);
					else if (decal.structure == olc::DecalStructure::STRIP)
						glBegin(GL_TRIANGLE_STRIP);
					else if (decal.structure == olc::DecalStructure::LIST)
						glBegin(GL_TRIANGLES);
				}


				// Render as 2D Spatial entity
				for (uint32_t n = 0; n < decal.points; n++)
				{
					glColor4ub(decal.tint[n].r, decal.tint[n].g, decal.tint[n].b, decal.tint[n].a);
					glTexCoord4f(decal.uv[n].x, decal.uv[n].y, 0.0f, decal.w[n]);
					glVertex2f(decal.pos[n].x, decal.pos[n].y);
				}


				glEnd();
			}



			//glDisable(GL_DEPTH_TEST);
		}


		uint32_t CreateTexture(const uint32_t width, const uint32_t height, const bool filtered, const bool clamp) override
		{
			UNUSED(width);
			UNUSED(height);
			uint32_t id = 0;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			if (filtered)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}


			if (clamp)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}


			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			return id;
		}


		uint32_t DeleteTexture(const uint32_t id) override
		{
			glDeleteTextures(1, &id);
			return id;
		}


		void UpdateTexture(uint32_t id, olc::Sprite* spr) override
		{
			UNUSED(id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spr->width, spr->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
		}


		void ReadTexture(uint32_t id, olc::Sprite* spr) override
		{
			glReadPixels(0, 0, spr->width, spr->height, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
		}


		void ApplyTexture(uint32_t id) override
		{
			glBindTexture(GL_TEXTURE_2D, id);
		}


		void ClearBuffer(olc::Pixel p, bool bDepth) override
		{
			glClearColor(float(p.r) / 255.0f, float(p.g) / 255.0f, float(p.b) / 255.0f, float(p.a) / 255.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			if (bDepth) glClear(GL_DEPTH_BUFFER_BIT);
		}


		void UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size) override
		{
			glViewport(pos.x, pos.y, size.x, size.y);
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END RENDERER: OpenGL 1.0 (the original, the best...)                         |
// O------------------------------------------------------------------------------O
#pragma endregion
#pragma region ogl33
// O------------------------------------------------------------------------------O
// | START RENDERER: OpenGL 3.3 (3.0 es) (sh-sh-sh-shaders....)                   |
// O------------------------------------------------------------------------------O
#if defined(OLC_GFX_OPENGL33)


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


	export class Renderer_OGL33 : public olc::Renderer
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
		void PrepareDevice() override
		{
#if defined(OLC_PLATFORM_GLUT)
			//glutInit has to be called with main() arguments, make fake ones
			int argc = 0;
			char* argv[1] = { (char*)"" };
			glutInit(&argc, argv);
			glutInitWindowPosition(0, 0);
			glutInitWindowSize(512, 512);
			glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
			// Creates the window and the OpenGL context for it
			glutCreateWindow("OneLoneCoder.com - Pixel Game Engine");
			glEnable(GL_TEXTURE_2D); // Turn on texturing
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
		}


		olc::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) override
		{
			// Create OpenGL Context
#if defined(OLC_PLATFORM_WINAPI)
			// Create Device Context
			glDeviceContext = GetDC((HWND)(params[0]));
			PIXELFORMATDESCRIPTOR pfd =
			{
				sizeof(PIXELFORMATDESCRIPTOR), 1,
				PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
				PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				PFD_MAIN_PLANE, 0, 0, 0, 0
			};


			int pf = 0;
			if (!(pf = ChoosePixelFormat(glDeviceContext, &pfd))) return olc::FAIL;
			SetPixelFormat(glDeviceContext, pf, &pfd);


			if (!(glRenderContext = wglCreateContext(glDeviceContext))) return olc::FAIL;
			wglMakeCurrent(glDeviceContext, glRenderContext);


			// Set Vertical Sync
			locSwapInterval = OGL_LOAD(locSwapInterval_t, wglSwapIntervalEXT);
			if (locSwapInterval && !bVSYNC) locSwapInterval(0);
			bSync = bVSYNC;
#endif


#if defined(OLC_PLATFORM_X11)
			using namespace X11;
			// Linux has tighter coupling between OpenGL and X11, so we store
			// various "platform" handles in the renderer
			olc_Display = (X11::Display*)(params[0]);
			olc_Window = (X11::Window*)(params[1]);
			olc_VisualInfo = (X11::XVisualInfo*)(params[2]);


			glDeviceContext = glXCreateContext(olc_Display, olc_VisualInfo, nullptr, GL_TRUE);
			glXMakeCurrent(olc_Display, *olc_Window, glDeviceContext);


			XWindowAttributes gwa;
			XGetWindowAttributes(olc_Display, *olc_Window, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);


			locSwapInterval = OGL_LOAD(locSwapInterval_t, glXSwapIntervalEXT);


			if (locSwapInterval == nullptr && !bVSYNC)
			{
				printf("NOTE: Could not disable VSYNC, glXSwapIntervalEXT() was not found!\n");
				printf("      Don't worry though, things will still work, it's just the\n");
				printf("      frame rate will be capped to your monitors refresh rate - javidx9\n");
			}


			if (locSwapInterval != nullptr && !bVSYNC)
				locSwapInterval(olc_Display, *olc_Window, 0);
#endif		


#if defined(OLC_PLATFORM_EMSCRIPTEN)
			EGLint const attribute_list[] = { EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
			EGLint const context_config[] = { EGL_CONTEXT_CLIENT_VERSION , 2, EGL_NONE };
			EGLint num_config;


			olc_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			eglInitialize(olc_Display, nullptr, nullptr);
			eglChooseConfig(olc_Display, attribute_list, &olc_Config, 1, &num_config);


			/* create an EGL rendering context */
			olc_Context = eglCreateContext(olc_Display, olc_Config, EGL_NO_CONTEXT, context_config);
			olc_Surface = eglCreateWindowSurface(olc_Display, olc_Config, NULL, nullptr);
			eglMakeCurrent(olc_Display, olc_Surface, olc_Surface, olc_Context);
			//eglSwapInterval is currently a NOP, plement anyways in case it becomes supported
			locSwapInterval = &eglSwapInterval;
			locSwapInterval(olc_Display, bVSYNC ? 1 : 0);
#endif


#if defined(OLC_PLATFORM_GLUT)
			mFullScreen = bFullScreen;
			if (!bVSYNC)
			{
#if defined(__APPLE__)
				GLint sync = 0;
				CGLContextObj ctx = CGLGetCurrentContext();
				if (ctx) CGLSetParameter(ctx, kCGLCPSwapInterval, &sync);
#endif
			}
#else
#if !defined(OLC_PLATFORM_EMSCRIPTEN)
			glEnable(GL_TEXTURE_2D); // Turn on texturing
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
#endif
#endif
			// Load External OpenGL Functions
			locCreateShader = OGL_LOAD(locCreateShader_t, glCreateShader);
			locCompileShader = OGL_LOAD(locCompileShader_t, glCompileShader);
			locShaderSource = OGL_LOAD(locShaderSource_t, glShaderSource);
			locDeleteShader = OGL_LOAD(locDeleteShader_t, glDeleteShader);
			locCreateProgram = OGL_LOAD(locCreateProgram_t, glCreateProgram);
			locDeleteProgram = OGL_LOAD(locDeleteProgram_t, glDeleteProgram);
			locLinkProgram = OGL_LOAD(locLinkProgram_t, glLinkProgram);
			locAttachShader = OGL_LOAD(locAttachShader_t, glAttachShader);
			locBindBuffer = OGL_LOAD(locBindBuffer_t, glBindBuffer);
			locBufferData = OGL_LOAD(locBufferData_t, glBufferData);
			locGenBuffers = OGL_LOAD(locGenBuffers_t, glGenBuffers);
			locVertexAttribPointer = OGL_LOAD(locVertexAttribPointer_t, glVertexAttribPointer);
			locEnableVertexAttribArray = OGL_LOAD(locEnableVertexAttribArray_t, glEnableVertexAttribArray);
			locUseProgram = OGL_LOAD(locUseProgram_t, glUseProgram);
			locGetShaderInfoLog = OGL_LOAD(locGetShaderInfoLog_t, glGetShaderInfoLog);
#if !defined(OLC_PLATFORM_EMSCRIPTEN)
			locBindVertexArray = OGL_LOAD(locBindVertexArray_t, glBindVertexArray);
			locGenVertexArrays = OGL_LOAD(locGenVertexArrays_t, glGenVertexArrays);
#else
			locBindVertexArray = glBindVertexArrayOES;
			locGenVertexArrays = glGenVertexArraysOES;
#endif


			// Load & Compile Quad Shader - assumes no errors
			m_nFS = locCreateShader(0x8B30);
			const GLchar* strFS =
#if defined(__arm__) || defined(OLC_PLATFORM_EMSCRIPTEN)
				"#version 300 es\n"
				"precision mediump float;"
#else
				"#version 330 core\n"
#endif
				"out vec4 pixel;\n""in vec2 oTex;\n"
				"in vec4 oCol;\n""uniform sampler2D sprTex;\n""void main(){pixel = texture(sprTex, oTex) * oCol;}";
			locShaderSource(m_nFS, 1, &strFS, NULL);
			locCompileShader(m_nFS);


			m_nVS = locCreateShader(0x8B31);
			const GLchar* strVS =
#if defined(__arm__) || defined(OLC_PLATFORM_EMSCRIPTEN)
				"#version 300 es\n"
				"precision mediump float;"
#else
				"#version 330 core\n"
#endif
				"layout(location = 0) in vec3 aPos;\n""layout(location = 1) in vec2 aTex;\n"
				"layout(location = 2) in vec4 aCol;\n""out vec2 oTex;\n""out vec4 oCol;\n"
				"void main(){ float p = 1.0 / aPos.z; gl_Position = p * vec4(aPos.x, aPos.y, 0.0, 1.0); oTex = p * aTex; oCol = aCol;}";
			locShaderSource(m_nVS, 1, &strVS, NULL);
			locCompileShader(m_nVS);


			m_nQuadShader = locCreateProgram();
			locAttachShader(m_nQuadShader, m_nFS);
			locAttachShader(m_nQuadShader, m_nVS);
			locLinkProgram(m_nQuadShader);


			// Create Quad
			locGenBuffers(1, &m_vbQuad);
			locGenVertexArrays(1, &m_vaQuad);
			locBindVertexArray(m_vaQuad);
			locBindBuffer(0x8892, m_vbQuad);


			locVertex verts[OLC_MAX_VERTS];
			locBufferData(0x8892, sizeof(locVertex) * OLC_MAX_VERTS, verts, 0x88E0);
			locVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(locVertex), 0); locEnableVertexAttribArray(0);
			locVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(locVertex), (void*)(3 * sizeof(float))); locEnableVertexAttribArray(1);
			locVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(locVertex), (void*)(5 * sizeof(float)));	locEnableVertexAttribArray(2);
			locBindBuffer(0x8892, 0);
			locBindVertexArray(0);


			// Create blank texture for spriteless decals
			rendBlankQuad.Create(1, 1);
			rendBlankQuad.Sprite()->GetData()[0] = olc::WHITE;
			rendBlankQuad.Decal()->Update();
			return olc::rcode::OK;
		}


		olc::rcode DestroyDevice() override
		{
#if defined(OLC_PLATFORM_WINAPI)
			wglDeleteContext(glRenderContext);
#endif


#if defined(OLC_PLATFORM_X11)
			glXMakeCurrent(olc_Display, None, NULL);
			glXDestroyContext(olc_Display, glDeviceContext);
#endif


#if defined(OLC_PLATFORM_GLUT)
			glutDestroyWindow(glutGetWindow());
#endif


#if defined(OLC_PLATFORM_EMSCRIPTEN)
			eglMakeCurrent(olc_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			eglDestroyContext(olc_Display, olc_Context);
			eglDestroySurface(olc_Display, olc_Surface);
			eglTerminate(olc_Display);
			olc_Display = EGL_NO_DISPLAY;
			olc_Surface = EGL_NO_SURFACE;
			olc_Context = EGL_NO_CONTEXT;
#endif
			return olc::rcode::OK;
		}


		void DisplayFrame() override
		{
#if defined(OLC_PLATFORM_WINAPI)
			SwapBuffers(glDeviceContext);
			if (bSync) DwmFlush(); // Woooohooooooo!!!! SMOOOOOOOTH!
#endif	


#if defined(OLC_PLATFORM_X11)
			X11::glXSwapBuffers(olc_Display, *olc_Window);
#endif		


#if defined(OLC_PLATFORM_GLUT)
			glutSwapBuffers();
#endif


#if defined(OLC_PLATFORM_EMSCRIPTEN)
			eglSwapBuffers(olc_Display, olc_Surface);
#endif
		}


		void PrepareDrawing() override
		{
			glEnable(GL_BLEND);
			nDecalMode = DecalMode::NORMAL;
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			locUseProgram(m_nQuadShader);
			locBindVertexArray(m_vaQuad);


#if defined(OLC_PLATFORM_EMSCRIPTEN)
			locVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(locVertex), 0); locEnableVertexAttribArray(0);
			locVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(locVertex), (void*)(3 * sizeof(float))); locEnableVertexAttribArray(1);
			locVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(locVertex), (void*)(5 * sizeof(float)));	locEnableVertexAttribArray(2);
#endif
		}


		void SetDecalMode(const olc::DecalMode& mode) override
		{
			if (mode != nDecalMode)
			{
				switch (mode)
				{
				case olc::DecalMode::NORMAL: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	break;
				case olc::DecalMode::ADDITIVE: glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
				case olc::DecalMode::MULTIPLICATIVE: glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);	break;
				case olc::DecalMode::STENCIL: glBlendFunc(GL_ZERO, GL_SRC_ALPHA); break;
				case olc::DecalMode::ILLUMINATE: glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);	break;
				case olc::DecalMode::WIREFRAME: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	break;
				}


				nDecalMode = mode;
			}
		}


		void DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint) override
		{
			locBindBuffer(0x8892, m_vbQuad);
			locVertex verts[4] = {
				{{-1.0f, -1.0f, 1.0}, {0.0f * scale.x + offset.x, 1.0f * scale.y + offset.y}, tint},
				{{+1.0f, -1.0f, 1.0}, {1.0f * scale.x + offset.x, 1.0f * scale.y + offset.y}, tint},
				{{-1.0f, +1.0f, 1.0}, {0.0f * scale.x + offset.x, 0.0f * scale.y + offset.y}, tint},
				{{+1.0f, +1.0f, 1.0}, {1.0f * scale.x + offset.x, 0.0f * scale.y + offset.y}, tint},
			};


			locBufferData(0x8892, sizeof(locVertex) * 4, verts, 0x88E0);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		}


		void DrawDecal(const olc::DecalInstance& decal) override
		{
			SetDecalMode(decal.mode);
			if (decal.decal == nullptr)
				glBindTexture(GL_TEXTURE_2D, rendBlankQuad.Decal()->id);
			else
				glBindTexture(GL_TEXTURE_2D, decal.decal->id);


			locBindBuffer(0x8892, m_vbQuad);


			for (uint32_t i = 0; i < decal.points; i++)
				pVertexMem[i] = { { decal.pos[i].x, decal.pos[i].y, decal.w[i] }, { decal.uv[i].x, decal.uv[i].y }, decal.tint[i] };


			locBufferData(0x8892, sizeof(locVertex) * decal.points, pVertexMem, 0x88E0);


			if (nDecalMode == DecalMode::WIREFRAME)
				glDrawArrays(GL_LINE_LOOP, 0, decal.points);
			else
				glDrawArrays(GL_TRIANGLE_FAN, 0, decal.points);
		}


		uint32_t CreateTexture(const uint32_t width, const uint32_t height, const bool filtered, const bool clamp) override
		{
			UNUSED(width);
			UNUSED(height);
			uint32_t id = 0;
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);


			if (filtered)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}


			if (clamp)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
#if !defined(OLC_PLATFORM_EMSCRIPTEN)
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#endif
			return id;
		}


		uint32_t DeleteTexture(const uint32_t id) override
		{
			glDeleteTextures(1, &id);
			return id;
		}


		void UpdateTexture(uint32_t id, olc::Sprite* spr) override
		{
			UNUSED(id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spr->width, spr->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
		}


		void ReadTexture(uint32_t id, olc::Sprite* spr) override
		{
			glReadPixels(0, 0, spr->width, spr->height, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
		}


		void ApplyTexture(uint32_t id) override
		{
			glBindTexture(GL_TEXTURE_2D, id);
		}


		void ClearBuffer(olc::Pixel p, bool bDepth) override
		{
			glClearColor(float(p.r) / 255.0f, float(p.g) / 255.0f, float(p.b) / 255.0f, float(p.a) / 255.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			if (bDepth) glClear(GL_DEPTH_BUFFER_BIT);
		}


		void UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size) override
		{
			glViewport(pos.x, pos.y, size.x, size.y);
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END RENDERER: OpenGL 3.3 (3.0 es) (sh-sh-sh-shaders....)                     |
// O------------------------------------------------------------------------------O
#pragma endregion
#pragma endregion