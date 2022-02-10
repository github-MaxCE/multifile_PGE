#pragma once
#ifndef OLCRENDERER_OGL10_H
#define OLCRENDERER_OGL10_H
#include "olcV2D.h"
#include "olcRenderer.h"

#pragma region renderer_ogl10
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
	class Renderer_OGL10 : public olc::Renderer
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
		void PrepareDevice() override;
		olc::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) override;
		olc::rcode DestroyDevice() override;
		void DisplayFrame() override;
		void PrepareDrawing() override;
		void SetDecalMode(const olc::DecalMode& mode);
		void DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint) override;
		void DrawDecal(const olc::DecalInstance& decal) override;
		uint32_t CreateTexture(const uint32_t width, const uint32_t height, const bool filtered, const bool clamp) override;
		void UpdateTexture(uint32_t id, olc::Sprite* spr) override;
		void ReadTexture(uint32_t id, olc::Sprite* spr) override;
		uint32_t DeleteTexture(const uint32_t id) override;
		void ApplyTexture(uint32_t id) override;
		void UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size) override;
		void ClearBuffer(olc::Pixel p, bool bDepth) override;
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END RENDERER: OpenGL 1.0 (the original, the best...)                         |
// O------------------------------------------------------------------------------O
#pragma endregion
#endif //OLCRENDERER_OGL10_H