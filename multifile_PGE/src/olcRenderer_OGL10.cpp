#include "olcRenderer_OGL10.h"

namespace olc
{
	void Renderer_OGL10::PrepareDevice()
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
	olc::rcode Renderer_OGL10::CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC)
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
	olc::rcode Renderer_OGL10::DestroyDevice()
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
	void Renderer_OGL10::DisplayFrame()
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
	void Renderer_OGL10::PrepareDrawing()
	{

		//ClearBuffer(olc::GREEN, true);
		glEnable(GL_BLEND);
		nDecalMode = DecalMode::NORMAL;
		nDecalStructure = DecalStructure::FAN;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	void Renderer_OGL10::SetDecalMode(const olc::DecalMode& mode)
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
	void Renderer_OGL10::DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint)
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
	void Renderer_OGL10::DrawDecal(const olc::DecalInstance& decal)
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
	uint32_t Renderer_OGL10::CreateTexture(const uint32_t width, const uint32_t height, const bool filtered, const bool clamp)
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
	void Renderer_OGL10::UpdateTexture(uint32_t id, olc::Sprite* spr)
	{
		UNUSED(id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spr->width, spr->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
	}
	void Renderer_OGL10::ReadTexture(uint32_t id, olc::Sprite* spr)
	{
		glReadPixels(0, 0, spr->width, spr->height, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
	}
	uint32_t Renderer_OGL10::DeleteTexture(const uint32_t id)
	{
		glDeleteTextures(1, &id);
		return id;
	}
	void Renderer_OGL10::ApplyTexture(uint32_t id)
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void Renderer_OGL10::UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size)
	{
		glViewport(pos.x, pos.y, size.x, size.y);
	}
	void Renderer_OGL10::ClearBuffer(olc::Pixel p, bool bDepth)
	{
		glClearColor(float(p.r) / 255.0f, float(p.g) / 255.0f, float(p.b) / 255.0f, float(p.a) / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		if (bDepth) glClear(GL_DEPTH_BUFFER_BIT);
	}
}