#include "olcRenderer_OGL33.h"

namespace olc
{
	void Renderer_OGL33::PrepareDevice()
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
	olc::rcode Renderer_OGL33::CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC)
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
		locSwapInterval = OGL_LOAD(locSwapInterval_t, "wglSwapIntervalEXT");
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

		locSwapInterval = OGL_LOAD(locSwapInterval_t, "glXSwapIntervalEXT");

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
	olc::rcode Renderer_OGL33::DestroyDevice()
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
	void Renderer_OGL33::DisplayFrame()
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
	void Renderer_OGL33::PrepareDrawing()
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
	void Renderer_OGL33::SetDecalMode(const olc::DecalMode& mode)
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
	void Renderer_OGL33::DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint)
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
	void Renderer_OGL33::DrawDecal(const olc::DecalInstance& decal)
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
	uint32_t Renderer_OGL33::CreateTexture(const uint32_t width, const uint32_t height, const bool filtered, const bool clamp)
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
	uint32_t Renderer_OGL33::DeleteTexture(const uint32_t id)
	{
		glDeleteTextures(1, &id);
		return id;
	}
	void Renderer_OGL33::UpdateTexture(uint32_t id, olc::Sprite* spr)
	{
		UNUSED(id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spr->width, spr->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
	}
	void Renderer_OGL33::ReadTexture(uint32_t id, olc::Sprite* spr)
	{
		glReadPixels(0, 0, spr->width, spr->height, GL_RGBA, GL_UNSIGNED_BYTE, spr->GetData());
	}
	void Renderer_OGL33::ApplyTexture(uint32_t id)
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}
	void Renderer_OGL33::ClearBuffer(olc::Pixel p, bool bDepth)
	{
		glClearColor(float(p.r) / 255.0f, float(p.g) / 255.0f, float(p.b) / 255.0f, float(p.a) / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		if (bDepth) glClear(GL_DEPTH_BUFFER_BIT);
	}
	void Renderer_OGL33::UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size)
	{
		glViewport(pos.x, pos.y, size.x, size.y);
	}
}