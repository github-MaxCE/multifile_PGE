module;

export module olc.Platform;

import std.core;
import olc.PGE;
import olc.xtra;
import olc.V2D;

namespace olc
{
	export class Platform
	{
	public:
		virtual ~Platform() = default;
		virtual olc::rcode ApplicationStartUp() = 0;
		virtual olc::rcode ApplicationCleanUp() = 0;
		virtual olc::rcode ThreadStartUp() = 0;
		virtual olc::rcode ThreadCleanUp() = 0;
		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) = 0;
		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) = 0;
		virtual olc::rcode SetWindowTitle(const std::string& s) = 0;
		virtual olc::rcode StartSystemEventLoop() = 0;
		virtual olc::rcode HandleSystemEvent() = 0;
		static olc::PixelGameEngine* ptrPGE;
	};

	olc::PixelGameEngine* olc::Platform::ptrPGE = nullptr;
}

// O------------------------------------------------------------------------------O
// | olcPixelGameEngine Platforms                                                 |
// O------------------------------------------------------------------------------O
#pragma region platforms
#pragma region windows
// O------------------------------------------------------------------------------O
// | START PLATFORM: MICROSOFT WINDOWS XP, VISTA, 7, 8, 10                        |
// O------------------------------------------------------------------------------O
#if defined(OLC_PLATFORM_WINAPI)


#if defined(_WIN32) && !defined(__MINGW32__)
#pragma comment(lib, "user32.lib")		// Visual Studio Only
#pragma comment(lib, "gdi32.lib")		// For other Windows Compilers please add
#pragma comment(lib, "opengl32.lib")	// these libs to your linker input
#endif


namespace olc
{
	export class Platform_Windows : public olc::Platform
	{
	private:
		HWND olc_hWnd = nullptr;
		std::wstring wsAppName;


		std::wstring ConvertS2W(std::string s)
		{
#ifdef __MINGW32__
			wchar_t* buffer = new wchar_t[s.length() + 1];
			mbstowcs(buffer, s.c_str(), s.length());
			buffer[s.length()] = L'\0';
#else
			int count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
			wchar_t* buffer = new wchar_t[count];
			MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
#endif
			std::wstring w(buffer);
			delete[] buffer;
			return w;
		}


	public:
		virtual olc::rcode ApplicationStartUp() override { return olc::rcode::OK; }
		virtual olc::rcode ApplicationCleanUp() override { return olc::rcode::OK; }
		virtual olc::rcode ThreadStartUp() override { return olc::rcode::OK; }


		virtual olc::rcode ThreadCleanUp() override
		{
			renderer->DestroyDevice();
			PostMessage(olc_hWnd, WM_DESTROY, 0, 0);
			return olc::OK;
		}


		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) override
		{
			if (renderer->CreateDevice({ olc_hWnd }, bFullScreen, bEnableVSYNC) == olc::rcode::OK)
			{
				renderer->UpdateViewport(vViewPos, vViewSize);
				return olc::rcode::OK;
			}
			else
				return olc::rcode::FAIL;
		}


		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) override
		{
			WNDCLASS wc;
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wc.hInstance = GetModuleHandle(nullptr);
			wc.lpfnWndProc = olc_WindowEvent;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.lpszMenuName = nullptr;
			wc.hbrBackground = nullptr;
			wc.lpszClassName = olcT("OLC_PIXEL_GAME_ENGINE");
			RegisterClass(&wc);


			// Define window furniture
			DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;


			olc::vi2d vTopLeft = vWindowPos;


			// Handle Fullscreen
			if (bFullScreen)
			{
				dwExStyle = 0;
				dwStyle = WS_VISIBLE | WS_POPUP;
				HMONITOR hmon = MonitorFromWindow(olc_hWnd, MONITOR_DEFAULTTONEAREST);
				MONITORINFO mi = { sizeof(mi) };
				if (!GetMonitorInfo(hmon, &mi)) return olc::rcode::FAIL;
				vWindowSize = { mi.rcMonitor.right, mi.rcMonitor.bottom };
				vTopLeft.x = 0;
				vTopLeft.y = 0;
			}


			// Keep client size as requested
			RECT rWndRect = { 0, 0, vWindowSize.x, vWindowSize.y };
			AdjustWindowRectEx(&rWndRect, dwStyle, FALSE, dwExStyle);
			int width = rWndRect.right - rWndRect.left;
			int height = rWndRect.bottom - rWndRect.top;


			olc_hWnd = CreateWindowEx(dwExStyle, olcT("OLC_PIXEL_GAME_ENGINE"), olcT(""), dwStyle,
				vTopLeft.x, vTopLeft.y, width, height, NULL, NULL, GetModuleHandle(nullptr), this);


			// Create Keyboard Mapping
			mapKeys[0x00] = Key::NONE;
			mapKeys[0x41] = Key::A; mapKeys[0x42] = Key::B; mapKeys[0x43] = Key::C; mapKeys[0x44] = Key::D; mapKeys[0x45] = Key::E;
			mapKeys[0x46] = Key::F; mapKeys[0x47] = Key::G; mapKeys[0x48] = Key::H; mapKeys[0x49] = Key::I; mapKeys[0x4A] = Key::J;
			mapKeys[0x4B] = Key::K; mapKeys[0x4C] = Key::L; mapKeys[0x4D] = Key::M; mapKeys[0x4E] = Key::N; mapKeys[0x4F] = Key::O;
			mapKeys[0x50] = Key::P; mapKeys[0x51] = Key::Q; mapKeys[0x52] = Key::R; mapKeys[0x53] = Key::S; mapKeys[0x54] = Key::T;
			mapKeys[0x55] = Key::U; mapKeys[0x56] = Key::V; mapKeys[0x57] = Key::W; mapKeys[0x58] = Key::X; mapKeys[0x59] = Key::Y;
			mapKeys[0x5A] = Key::Z;


			mapKeys[VK_F1] = Key::F1; mapKeys[VK_F2] = Key::F2; mapKeys[VK_F3] = Key::F3; mapKeys[VK_F4] = Key::F4;
			mapKeys[VK_F5] = Key::F5; mapKeys[VK_F6] = Key::F6; mapKeys[VK_F7] = Key::F7; mapKeys[VK_F8] = Key::F8;
			mapKeys[VK_F9] = Key::F9; mapKeys[VK_F10] = Key::F10; mapKeys[VK_F11] = Key::F11; mapKeys[VK_F12] = Key::F12;


			mapKeys[VK_DOWN] = Key::DOWN; mapKeys[VK_LEFT] = Key::LEFT; mapKeys[VK_RIGHT] = Key::RIGHT; mapKeys[VK_UP] = Key::UP;
			//mapKeys[VK_RETURN] = Key::ENTER;// mapKeys[VK_RETURN] = Key::RETURN;

			mapKeys[VK_BACK] = Key::BACK; mapKeys[VK_ESCAPE] = Key::ESCAPE; mapKeys[VK_RETURN] = Key::ENTER; mapKeys[VK_PAUSE] = Key::PAUSE;
			mapKeys[VK_SCROLL] = Key::SCROLL; mapKeys[VK_TAB] = Key::TAB; mapKeys[VK_DELETE] = Key::DEL; mapKeys[VK_HOME] = Key::HOME;
			mapKeys[VK_END] = Key::END; mapKeys[VK_PRIOR] = Key::PGUP; mapKeys[VK_NEXT] = Key::PGDN; mapKeys[VK_INSERT] = Key::INS;
			mapKeys[VK_SHIFT] = Key::SHIFT; mapKeys[VK_CONTROL] = Key::CTRL;
			mapKeys[VK_SPACE] = Key::SPACE;


			mapKeys[0x30] = Key::K0; mapKeys[0x31] = Key::K1; mapKeys[0x32] = Key::K2; mapKeys[0x33] = Key::K3; mapKeys[0x34] = Key::K4;
			mapKeys[0x35] = Key::K5; mapKeys[0x36] = Key::K6; mapKeys[0x37] = Key::K7; mapKeys[0x38] = Key::K8; mapKeys[0x39] = Key::K9;


			mapKeys[VK_NUMPAD0] = Key::NP0; mapKeys[VK_NUMPAD1] = Key::NP1; mapKeys[VK_NUMPAD2] = Key::NP2; mapKeys[VK_NUMPAD3] = Key::NP3; mapKeys[VK_NUMPAD4] = Key::NP4;
			mapKeys[VK_NUMPAD5] = Key::NP5; mapKeys[VK_NUMPAD6] = Key::NP6; mapKeys[VK_NUMPAD7] = Key::NP7; mapKeys[VK_NUMPAD8] = Key::NP8; mapKeys[VK_NUMPAD9] = Key::NP9;
			mapKeys[VK_MULTIPLY] = Key::NP_MUL; mapKeys[VK_ADD] = Key::NP_ADD; mapKeys[VK_DIVIDE] = Key::NP_DIV; mapKeys[VK_SUBTRACT] = Key::NP_SUB; mapKeys[VK_DECIMAL] = Key::NP_DECIMAL;


			// Thanks scripticuk
			mapKeys[VK_OEM_1] = Key::OEM_1;			// On US and UK keyboards this is the ';:' key
			mapKeys[VK_OEM_2] = Key::OEM_2;			// On US and UK keyboards this is the '/?' key
			mapKeys[VK_OEM_3] = Key::OEM_3;			// On US keyboard this is the '~' key
			mapKeys[VK_OEM_4] = Key::OEM_4;			// On US and UK keyboards this is the '[{' key
			mapKeys[VK_OEM_5] = Key::OEM_5;			// On US keyboard this is '\|' key.
			mapKeys[VK_OEM_6] = Key::OEM_6;			// On US and UK keyboards this is the ']}' key
			mapKeys[VK_OEM_7] = Key::OEM_7;			// On US keyboard this is the single/double quote key. On UK, this is the single quote/@ symbol key
			mapKeys[VK_OEM_8] = Key::OEM_8;			// miscellaneous characters. Varies by keyboard
			mapKeys[VK_OEM_PLUS] = Key::EQUALS;		// the '+' key on any keyboard
			mapKeys[VK_OEM_COMMA] = Key::COMMA;		// the comma key on any keyboard
			mapKeys[VK_OEM_MINUS] = Key::MINUS;		// the minus key on any keyboard
			mapKeys[VK_OEM_PERIOD] = Key::PERIOD;	// the period key on any keyboard
			mapKeys[VK_CAPITAL] = Key::CAPS_LOCK;
			return olc::OK;
		}


		virtual olc::rcode SetWindowTitle(const std::string& s) override
		{
#ifdef UNICODE
			SetWindowText(olc_hWnd, ConvertS2W(s).c_str());
#else
			SetWindowText(olc_hWnd, s.c_str());
#endif
			return olc::OK;
		}


		virtual olc::rcode StartSystemEventLoop() override
		{
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return olc::OK;
		}


		virtual olc::rcode HandleSystemEvent() override { return olc::rcode::FAIL; }


		// Windows Event Handler - this is statically connected to the windows event system
		static LRESULT CALLBACK olc_WindowEvent(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			switch (uMsg)
			{
			case WM_MOUSEMOVE:
			{
				// Thanks @ForAbby (Discord)
				uint16_t x = lParam & 0xFFFF; uint16_t y = (lParam >> 16) & 0xFFFF;
				int16_t ix = *(int16_t*)&x;   int16_t iy = *(int16_t*)&y;
				ptrPGE->olc_UpdateMouse(ix, iy);
				return 0;
			}
			case WM_SIZE:       ptrPGE->olc_UpdateWindowSize(lParam & 0xFFFF, (lParam >> 16) & 0xFFFF);	return 0;
			case WM_MOUSEWHEEL:	ptrPGE->olc_UpdateMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));           return 0;
			case WM_MOUSELEAVE: ptrPGE->olc_UpdateMouseFocus(false);                                    return 0;
			case WM_SETFOCUS:	ptrPGE->olc_UpdateKeyFocus(true);                                       return 0;
			case WM_KILLFOCUS:	ptrPGE->olc_UpdateKeyFocus(false);                                      return 0;
			case WM_KEYDOWN:	ptrPGE->olc_UpdateKeyState(mapKeys[wParam], true);                      return 0;
			case WM_KEYUP:		ptrPGE->olc_UpdateKeyState(mapKeys[wParam], false);                     return 0;
			case WM_SYSKEYDOWN: ptrPGE->olc_UpdateKeyState(mapKeys[wParam], true);						return 0;
			case WM_SYSKEYUP:	ptrPGE->olc_UpdateKeyState(mapKeys[wParam], false);						return 0;
			case WM_LBUTTONDOWN:ptrPGE->olc_UpdateMouseState(0, true);                                  return 0;
			case WM_LBUTTONUP:	ptrPGE->olc_UpdateMouseState(0, false);                                 return 0;
			case WM_RBUTTONDOWN:ptrPGE->olc_UpdateMouseState(1, true);                                  return 0;
			case WM_RBUTTONUP:	ptrPGE->olc_UpdateMouseState(1, false);                                 return 0;
			case WM_MBUTTONDOWN:ptrPGE->olc_UpdateMouseState(2, true);                                  return 0;
			case WM_MBUTTONUP:	ptrPGE->olc_UpdateMouseState(2, false);                                 return 0;
			case WM_CLOSE:		ptrPGE->olc_Terminate();                                                return 0;
			case WM_DESTROY:	PostQuitMessage(0); DestroyWindow(hWnd);								return 0;
			}
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END PLATFORM: MICROSOFT WINDOWS XP, VISTA, 7, 8, 10                          |
// O------------------------------------------------------------------------------O
#pragma endregion 
#pragma region linux
// O------------------------------------------------------------------------------O
// | START PLATFORM: LINUX                                                        |
// O------------------------------------------------------------------------------O
#if defined(OLC_PLATFORM_X11)
namespace olc
{
	export class Platform_Linux : public olc::Platform
	{
	private:
		X11::Display* olc_Display = nullptr;
		X11::Window					 olc_WindowRoot;
		X11::Window					 olc_Window;
		X11::XVisualInfo* olc_VisualInfo;
		X11::Colormap                olc_ColourMap;
		X11::XSetWindowAttributes    olc_SetWindowAttribs;


	public:
		virtual olc::rcode ApplicationStartUp() override
		{
			return olc::rcode::OK;
		}


		virtual olc::rcode ApplicationCleanUp() override
		{
			XDestroyWindow(olc_Display, olc_Window);
			return olc::rcode::OK;
		}


		virtual olc::rcode ThreadStartUp() override
		{
			return olc::rcode::OK;
		}


		virtual olc::rcode ThreadCleanUp() override
		{
			renderer->DestroyDevice();
			return olc::OK;
		}


		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) override
		{
			if (renderer->CreateDevice({ olc_Display, &olc_Window, olc_VisualInfo }, bFullScreen, bEnableVSYNC) == olc::rcode::OK)
			{
				renderer->UpdateViewport(vViewPos, vViewSize);
				return olc::rcode::OK;
			}
			else
				return olc::rcode::FAIL;
		}


		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) override
		{
			using namespace X11;
			XInitThreads();


			// Grab the deafult display and window
			olc_Display = XOpenDisplay(NULL);
			olc_WindowRoot = DefaultRootWindow(olc_Display);


			// Based on the display capabilities, configure the appearance of the window
			GLint olc_GLAttribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
			olc_VisualInfo = glXChooseVisual(olc_Display, 0, olc_GLAttribs);
			olc_ColourMap = XCreateColormap(olc_Display, olc_WindowRoot, olc_VisualInfo->visual, AllocNone);
			olc_SetWindowAttribs.colormap = olc_ColourMap;


			// Register which events we are interested in receiving
			olc_SetWindowAttribs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
				ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask | StructureNotifyMask;


			// Create the window
			olc_Window = XCreateWindow(olc_Display, olc_WindowRoot, vWindowPos.x, vWindowPos.y,
				vWindowSize.x, vWindowSize.y,
				0, olc_VisualInfo->depth, InputOutput, olc_VisualInfo->visual,
				CWColormap | CWEventMask, &olc_SetWindowAttribs);


			Atom wmDelete = XInternAtom(olc_Display, "WM_DELETE_WINDOW", true);
			XSetWMProtocols(olc_Display, olc_Window, &wmDelete, 1);


			XMapWindow(olc_Display, olc_Window);
			XStoreName(olc_Display, olc_Window, "OneLoneCoder.com - Pixel Game Engine");


			if (bFullScreen) // Thanks DragonEye, again :D
			{
				Atom wm_state;
				Atom fullscreen;
				wm_state = XInternAtom(olc_Display, "_NET_WM_STATE", False);
				fullscreen = XInternAtom(olc_Display, "_NET_WM_STATE_FULLSCREEN", False);
				XEvent xev{ 0 };
				xev.type = ClientMessage;
				xev.xclient.window = olc_Window;
				xev.xclient.message_type = wm_state;
				xev.xclient.format = 32;
				xev.xclient.data.l[0] = (bFullScreen ? 1 : 0);   // the action (0: off, 1: on, 2: toggle)
				xev.xclient.data.l[1] = fullscreen;             // first property to alter
				xev.xclient.data.l[2] = 0;                      // second property to alter
				xev.xclient.data.l[3] = 0;                      // source indication
				XMapWindow(olc_Display, olc_Window);
				XSendEvent(olc_Display, DefaultRootWindow(olc_Display), False,
					SubstructureRedirectMask | SubstructureNotifyMask, &xev);
				XFlush(olc_Display);
				XWindowAttributes gwa;
				XGetWindowAttributes(olc_Display, olc_Window, &gwa);
				vWindowSize.x = gwa.width;
				vWindowSize.y = gwa.height;
			}


			// Create Keyboard Mapping
			mapKeys[0x00] = Key::NONE;
			mapKeys[0x61] = Key::A; mapKeys[0x62] = Key::B; mapKeys[0x63] = Key::C; mapKeys[0x64] = Key::D; mapKeys[0x65] = Key::E;
			mapKeys[0x66] = Key::F; mapKeys[0x67] = Key::G; mapKeys[0x68] = Key::H; mapKeys[0x69] = Key::I; mapKeys[0x6A] = Key::J;
			mapKeys[0x6B] = Key::K; mapKeys[0x6C] = Key::L; mapKeys[0x6D] = Key::M; mapKeys[0x6E] = Key::N; mapKeys[0x6F] = Key::O;
			mapKeys[0x70] = Key::P; mapKeys[0x71] = Key::Q; mapKeys[0x72] = Key::R; mapKeys[0x73] = Key::S; mapKeys[0x74] = Key::T;
			mapKeys[0x75] = Key::U; mapKeys[0x76] = Key::V; mapKeys[0x77] = Key::W; mapKeys[0x78] = Key::X; mapKeys[0x79] = Key::Y;
			mapKeys[0x7A] = Key::Z;


			mapKeys[XK_F1] = Key::F1; mapKeys[XK_F2] = Key::F2; mapKeys[XK_F3] = Key::F3; mapKeys[XK_F4] = Key::F4;
			mapKeys[XK_F5] = Key::F5; mapKeys[XK_F6] = Key::F6; mapKeys[XK_F7] = Key::F7; mapKeys[XK_F8] = Key::F8;
			mapKeys[XK_F9] = Key::F9; mapKeys[XK_F10] = Key::F10; mapKeys[XK_F11] = Key::F11; mapKeys[XK_F12] = Key::F12;


			mapKeys[XK_Down] = Key::DOWN; mapKeys[XK_Left] = Key::LEFT; mapKeys[XK_Right] = Key::RIGHT; mapKeys[XK_Up] = Key::UP;
			mapKeys[XK_KP_Enter] = Key::ENTER; mapKeys[XK_Return] = Key::ENTER;


			mapKeys[XK_BackSpace] = Key::BACK; mapKeys[XK_Escape] = Key::ESCAPE; mapKeys[XK_Linefeed] = Key::ENTER;	mapKeys[XK_Pause] = Key::PAUSE;
			mapKeys[XK_Scroll_Lock] = Key::SCROLL; mapKeys[XK_Tab] = Key::TAB; mapKeys[XK_Delete] = Key::DEL; mapKeys[XK_Home] = Key::HOME;
			mapKeys[XK_End] = Key::END; mapKeys[XK_Page_Up] = Key::PGUP; mapKeys[XK_Page_Down] = Key::PGDN;	mapKeys[XK_Insert] = Key::INS;
			mapKeys[XK_Shift_L] = Key::SHIFT; mapKeys[XK_Shift_R] = Key::SHIFT; mapKeys[XK_Control_L] = Key::CTRL; mapKeys[XK_Control_R] = Key::CTRL;
			mapKeys[XK_space] = Key::SPACE; mapKeys[XK_period] = Key::PERIOD;


			mapKeys[XK_0] = Key::K0; mapKeys[XK_1] = Key::K1; mapKeys[XK_2] = Key::K2; mapKeys[XK_3] = Key::K3; mapKeys[XK_4] = Key::K4;
			mapKeys[XK_5] = Key::K5; mapKeys[XK_6] = Key::K6; mapKeys[XK_7] = Key::K7; mapKeys[XK_8] = Key::K8; mapKeys[XK_9] = Key::K9;


			mapKeys[XK_KP_0] = Key::NP0; mapKeys[XK_KP_1] = Key::NP1; mapKeys[XK_KP_2] = Key::NP2; mapKeys[XK_KP_3] = Key::NP3; mapKeys[XK_KP_4] = Key::NP4;
			mapKeys[XK_KP_5] = Key::NP5; mapKeys[XK_KP_6] = Key::NP6; mapKeys[XK_KP_7] = Key::NP7; mapKeys[XK_KP_8] = Key::NP8; mapKeys[XK_KP_9] = Key::NP9;
			mapKeys[XK_KP_Multiply] = Key::NP_MUL; mapKeys[XK_KP_Add] = Key::NP_ADD; mapKeys[XK_KP_Divide] = Key::NP_DIV; mapKeys[XK_KP_Subtract] = Key::NP_SUB; mapKeys[XK_KP_Decimal] = Key::NP_DECIMAL;


			// These keys vary depending on the keyboard. I've included comments for US and UK keyboard layouts
			mapKeys[XK_semicolon] = Key::OEM_1;		// On US and UK keyboards this is the ';:' key
			mapKeys[XK_slash] = Key::OEM_2;			// On US and UK keyboards this is the '/?' key
			mapKeys[XK_asciitilde] = Key::OEM_3;	// On US keyboard this is the '~' key
			mapKeys[XK_bracketleft] = Key::OEM_4;	// On US and UK keyboards this is the '[{' key
			mapKeys[XK_backslash] = Key::OEM_5;		// On US keyboard this is '\|' key.
			mapKeys[XK_bracketright] = Key::OEM_6;	// On US and UK keyboards this is the ']}' key
			mapKeys[XK_apostrophe] = Key::OEM_7;	// On US keyboard this is the single/double quote key. On UK, this is the single quote/@ symbol key
			mapKeys[XK_numbersign] = Key::OEM_8;	// miscellaneous characters. Varies by keyboard. I believe this to be the '#~' key on UK keyboards
			mapKeys[XK_equal] = Key::EQUALS;		// the '+' key on any keyboard
			mapKeys[XK_comma] = Key::COMMA;			// the comma key on any keyboard
			mapKeys[XK_minus] = Key::MINUS;			// the minus key on any keyboard			


			mapKeys[XK_Caps_Lock] = Key::CAPS_LOCK;


			return olc::OK;
		}


		virtual olc::rcode SetWindowTitle(const std::string& s) override
		{
			X11::XStoreName(olc_Display, olc_Window, s.c_str());
			return olc::OK;
		}


		virtual olc::rcode StartSystemEventLoop() override
		{
			return olc::OK;
		}


		virtual olc::rcode HandleSystemEvent() override
		{
			using namespace X11;
			// Handle Xlib Message Loop - we do this in the
			// same thread that OpenGL was created so we dont
			// need to worry too much about multithreading with X11
			XEvent xev;
			while (XPending(olc_Display))
			{
				XNextEvent(olc_Display, &xev);
				if (xev.type == Expose)
				{
					XWindowAttributes gwa;
					XGetWindowAttributes(olc_Display, olc_Window, &gwa);
					ptrPGE->olc_UpdateWindowSize(gwa.width, gwa.height);
				}
				else if (xev.type == ConfigureNotify)
				{
					XConfigureEvent xce = xev.xconfigure;
					ptrPGE->olc_UpdateWindowSize(xce.width, xce.height);
				}
				else if (xev.type == KeyPress)
				{
					KeySym sym = XLookupKeysym(&xev.xkey, 0);
					ptrPGE->olc_UpdateKeyState(mapKeys[sym], true);
					XKeyEvent* e = (XKeyEvent*)&xev; // Because DragonEye loves numpads
					XLookupString(e, NULL, 0, &sym, NULL);
					ptrPGE->olc_UpdateKeyState(mapKeys[sym], true);
				}
				else if (xev.type == KeyRelease)
				{
					KeySym sym = XLookupKeysym(&xev.xkey, 0);
					ptrPGE->olc_UpdateKeyState(mapKeys[sym], false);
					XKeyEvent* e = (XKeyEvent*)&xev;
					XLookupString(e, NULL, 0, &sym, NULL);
					ptrPGE->olc_UpdateKeyState(mapKeys[sym], false);
				}
				else if (xev.type == ButtonPress)
				{
					switch (xev.xbutton.button)
					{
					case 1:	ptrPGE->olc_UpdateMouseState(0, true); break;
					case 2:	ptrPGE->olc_UpdateMouseState(2, true); break;
					case 3:	ptrPGE->olc_UpdateMouseState(1, true); break;
					case 4:	ptrPGE->olc_UpdateMouseWheel(120); break;
					case 5:	ptrPGE->olc_UpdateMouseWheel(-120); break;
					default: break;
					}
				}
				else if (xev.type == ButtonRelease)
				{
					switch (xev.xbutton.button)
					{
					case 1:	ptrPGE->olc_UpdateMouseState(0, false); break;
					case 2:	ptrPGE->olc_UpdateMouseState(2, false); break;
					case 3:	ptrPGE->olc_UpdateMouseState(1, false); break;
					default: break;
					}
				}
				else if (xev.type == MotionNotify)
				{
					ptrPGE->olc_UpdateMouse(xev.xmotion.x, xev.xmotion.y);
				}
				else if (xev.type == FocusIn)
				{
					ptrPGE->olc_UpdateKeyFocus(true);
				}
				else if (xev.type == FocusOut)
				{
					ptrPGE->olc_UpdateKeyFocus(false);
				}
				else if (xev.type == ClientMessage)
				{
					ptrPGE->olc_Terminate();
				}
			}
			return olc::OK;
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END PLATFORM: LINUX                                                          |
// O------------------------------------------------------------------------------O
#pragma endregion
#pragma region glut
// O------------------------------------------------------------------------------O
// | START PLATFORM: GLUT (used to make it simple for Apple)                      |
// O------------------------------------------------------------------------------O
//
// VERY IMPORTANT!!! The Apple port was originally created by @Mumflr (discord)
// and the repo for the development of this project can be found here:
// https://github.com/MumflrFumperdink/olcPGEMac which contains maccy goodness
// and support on how to setup your build environment.
//
// "MASSIVE MASSIVE THANKS TO MUMFLR" - Javidx9
#if defined(OLC_PLATFORM_GLUT)
namespace olc {


	export class Platform_GLUT : public olc::Platform
	{
	public:
		static std::atomic<bool>* bActiveRef;


		virtual olc::rcode ApplicationStartUp() override {
			return olc::rcode::OK;
		}


		virtual olc::rcode ApplicationCleanUp() override
		{
			return olc::rcode::OK;
		}


		virtual olc::rcode ThreadStartUp() override
		{
			return olc::rcode::OK;
		}


		virtual olc::rcode ThreadCleanUp() override
		{
			renderer->DestroyDevice();
			return olc::OK;
		}


		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) override
		{
			if (renderer->CreateDevice({}, bFullScreen, bEnableVSYNC) == olc::rcode::OK)
			{
				renderer->UpdateViewport(vViewPos, vViewSize);
				return olc::rcode::OK;
			}
			else
				return olc::rcode::FAIL;
		}


		static void ExitMainLoop() {
			if (!ptrPGE->OnUserDestroy()) {
				*bActiveRef = true;
				return;
			}
			platform->ThreadCleanUp();
			platform->ApplicationCleanUp();
			exit(0);
		}


#if defined(__APPLE__)
		static void scrollWheelUpdate(id selff, SEL _sel, id theEvent) {
			static const SEL deltaYSel = sel_registerName("deltaY");


			double deltaY = ((double (*)(id, SEL))objc_msgSend_fpret)(theEvent, deltaYSel);


			for (int i = 0; i < abs(deltaY); i++) {
				if (deltaY > 0) {
					ptrPGE->olc_UpdateMouseWheel(-1);
				}
				else if (deltaY < 0) {
					ptrPGE->olc_UpdateMouseWheel(1);
				}
			}
		}
#endif
		static void ThreadFunct() {
#if defined(__APPLE__)
			static bool hasEnabledCocoa = false;
			if (!hasEnabledCocoa) {
				// Objective-C Wizardry
				Class NSApplicationClass = objc_getClass("NSApplication");


				// NSApp = [NSApplication sharedApplication]
				SEL sharedApplicationSel = sel_registerName("sharedApplication");
				id NSApp = ((id(*)(Class, SEL))objc_msgSend)(NSApplicationClass, sharedApplicationSel);
				// window = [NSApp mainWindow]
				SEL mainWindowSel = sel_registerName("mainWindow");
				id window = ((id(*)(id, SEL))objc_msgSend)(NSApp, mainWindowSel);


				// [window setStyleMask: NSWindowStyleMaskClosable | ~NSWindowStyleMaskResizable]
				SEL setStyleMaskSel = sel_registerName("setStyleMask:");
				((void (*)(id, SEL, NSUInteger))objc_msgSend)(window, setStyleMaskSel, 7);


				hasEnabledCocoa = true;
			}
#endif
			if (!*bActiveRef) {
				ExitMainLoop();
				return;
			}
			glutPostRedisplay();
		}


		static void DrawFunct() {
			ptrPGE->olc_CoreUpdate();
		}


		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) override
		{
#if defined(__APPLE__)
			Class GLUTViewClass = objc_getClass("GLUTView");


			SEL scrollWheelSel = sel_registerName("scrollWheel:");
			bool resultAddMethod = class_addMethod(GLUTViewClass, scrollWheelSel, (IMP)scrollWheelUpdate, "v@:@");
			assert(resultAddMethod);
#endif


			renderer->PrepareDevice();


			if (bFullScreen)
			{
				vWindowSize.x = glutGet(GLUT_SCREEN_WIDTH);
				vWindowSize.y = glutGet(GLUT_SCREEN_HEIGHT);
				glutFullScreen();
			}
			else
			{
				if (vWindowSize.x > glutGet(GLUT_SCREEN_WIDTH) || vWindowSize.y > glutGet(GLUT_SCREEN_HEIGHT))
				{
					perror("ERROR: The specified window dimensions do not fit on your screen\n");
					return olc::FAIL;
				}
				glutReshapeWindow(vWindowSize.x, vWindowSize.y - 1);
			}


			// Create Keyboard Mapping
			mapKeys[0x00] = Key::NONE;
			mapKeys['A'] = Key::A; mapKeys['B'] = Key::B; mapKeys['C'] = Key::C; mapKeys['D'] = Key::D; mapKeys['E'] = Key::E;
			mapKeys['F'] = Key::F; mapKeys['G'] = Key::G; mapKeys['H'] = Key::H; mapKeys['I'] = Key::I; mapKeys['J'] = Key::J;
			mapKeys['K'] = Key::K; mapKeys['L'] = Key::L; mapKeys['M'] = Key::M; mapKeys['N'] = Key::N; mapKeys['O'] = Key::O;
			mapKeys['P'] = Key::P; mapKeys['Q'] = Key::Q; mapKeys['R'] = Key::R; mapKeys['S'] = Key::S; mapKeys['T'] = Key::T;
			mapKeys['U'] = Key::U; mapKeys['V'] = Key::V; mapKeys['W'] = Key::W; mapKeys['X'] = Key::X; mapKeys['Y'] = Key::Y;
			mapKeys['Z'] = Key::Z;


			mapKeys[GLUT_KEY_F1] = Key::F1; mapKeys[GLUT_KEY_F2] = Key::F2; mapKeys[GLUT_KEY_F3] = Key::F3; mapKeys[GLUT_KEY_F4] = Key::F4;
			mapKeys[GLUT_KEY_F5] = Key::F5; mapKeys[GLUT_KEY_F6] = Key::F6; mapKeys[GLUT_KEY_F7] = Key::F7; mapKeys[GLUT_KEY_F8] = Key::F8;
			mapKeys[GLUT_KEY_F9] = Key::F9; mapKeys[GLUT_KEY_F10] = Key::F10; mapKeys[GLUT_KEY_F11] = Key::F11; mapKeys[GLUT_KEY_F12] = Key::F12;


			mapKeys[GLUT_KEY_DOWN] = Key::DOWN; mapKeys[GLUT_KEY_LEFT] = Key::LEFT; mapKeys[GLUT_KEY_RIGHT] = Key::RIGHT; mapKeys[GLUT_KEY_UP] = Key::UP;
			mapKeys[13] = Key::ENTER;


			mapKeys[127] = Key::BACK; mapKeys[27] = Key::ESCAPE;
			mapKeys[9] = Key::TAB;  mapKeys[GLUT_KEY_HOME] = Key::HOME;
			mapKeys[GLUT_KEY_END] = Key::END; mapKeys[GLUT_KEY_PAGE_UP] = Key::PGUP; mapKeys[GLUT_KEY_PAGE_DOWN] = Key::PGDN;    mapKeys[GLUT_KEY_INSERT] = Key::INS;
			mapKeys[32] = Key::SPACE; mapKeys[46] = Key::PERIOD;


			mapKeys[48] = Key::K0; mapKeys[49] = Key::K1; mapKeys[50] = Key::K2; mapKeys[51] = Key::K3; mapKeys[52] = Key::K4;
			mapKeys[53] = Key::K5; mapKeys[54] = Key::K6; mapKeys[55] = Key::K7; mapKeys[56] = Key::K8; mapKeys[57] = Key::K9;


			// NOTE: MISSING KEYS :O


			glutKeyboardFunc([](unsigned char key, int x, int y) -> void {
				switch (glutGetModifiers()) {
				case 0: //This is when there are no modifiers
					if ('a' <= key && key <= 'z') key -= 32;
					break;
				case GLUT_ACTIVE_SHIFT:
					ptrPGE->olc_UpdateKeyState(Key::SHIFT, true);
					break;
				case GLUT_ACTIVE_CTRL:
					if ('a' <= key && key <= 'z') key -= 32;
					ptrPGE->olc_UpdateKeyState(Key::CTRL, true);
					break;
				case GLUT_ACTIVE_ALT:
					if ('a' <= key && key <= 'z') key -= 32;
					break;
				}


				if (mapKeys[key])
					ptrPGE->olc_UpdateKeyState(mapKeys[key], true);
				});


			glutKeyboardUpFunc([](unsigned char key, int x, int y) -> void {
				switch (glutGetModifiers()) {
				case 0: //This is when there are no modifiers
					if ('a' <= key && key <= 'z') key -= 32;
					break;
				case GLUT_ACTIVE_SHIFT:
					ptrPGE->olc_UpdateKeyState(Key::SHIFT, false);
					break;
				case GLUT_ACTIVE_CTRL:
					if ('a' <= key && key <= 'z') key -= 32;
					ptrPGE->olc_UpdateKeyState(Key::CTRL, false);
					break;
				case GLUT_ACTIVE_ALT:
					if ('a' <= key && key <= 'z') key -= 32;
					//No ALT in PGE
					break;
				}


				if (mapKeys[key])
					ptrPGE->olc_UpdateKeyState(mapKeys[key], false);
				});


			//Special keys
			glutSpecialFunc([](int key, int x, int y) -> void {
				if (mapKeys[key])
					ptrPGE->olc_UpdateKeyState(mapKeys[key], true);
				});


			glutSpecialUpFunc([](int key, int x, int y) -> void {
				if (mapKeys[key])
					ptrPGE->olc_UpdateKeyState(mapKeys[key], false);
				});


			glutMouseFunc([](int button, int state, int x, int y) -> void {
				switch (button) {
				case GLUT_LEFT_BUTTON:
					if (state == GLUT_UP) ptrPGE->olc_UpdateMouseState(0, false);
					else if (state == GLUT_DOWN) ptrPGE->olc_UpdateMouseState(0, true);
					break;
				case GLUT_MIDDLE_BUTTON:
					if (state == GLUT_UP) ptrPGE->olc_UpdateMouseState(2, false);
					else if (state == GLUT_DOWN) ptrPGE->olc_UpdateMouseState(2, true);
					break;
				case GLUT_RIGHT_BUTTON:
					if (state == GLUT_UP) ptrPGE->olc_UpdateMouseState(1, false);
					else if (state == GLUT_DOWN) ptrPGE->olc_UpdateMouseState(1, true);
					break;
				}
				});


			auto mouseMoveCall = [](int x, int y) -> void {
				ptrPGE->olc_UpdateMouse(x, y);
			};


			glutMotionFunc(mouseMoveCall);
			glutPassiveMotionFunc(mouseMoveCall);


			glutEntryFunc([](int state) -> void {
				if (state == GLUT_ENTERED) ptrPGE->olc_UpdateKeyFocus(true);
				else if (state == GLUT_LEFT) ptrPGE->olc_UpdateKeyFocus(false);
				});


			glutDisplayFunc(DrawFunct);
			glutIdleFunc(ThreadFunct);


			return olc::OK;
		}


		virtual olc::rcode SetWindowTitle(const std::string& s) override
		{
			glutSetWindowTitle(s.c_str());
			return olc::OK;
		}


		virtual olc::rcode StartSystemEventLoop() override {
			glutMainLoop();
			return olc::OK;
		}


		virtual olc::rcode HandleSystemEvent() override
		{
			return olc::OK;
		}
	};


	std::atomic<bool>* Platform_GLUT::bActiveRef{ nullptr };


	//Custom Start
	olc::rcode PixelGameEngine::Start()
	{
		if (platform->ApplicationStartUp() != olc::OK) return olc::FAIL;


		// Construct the window
		if (platform->CreateWindowPane({ 30,30 }, vWindowSize, bFullScreen) != olc::OK) return olc::FAIL;
		olc_UpdateWindowSize(vWindowSize.x, vWindowSize.y);


		if (platform->ThreadStartUp() == olc::FAIL)  return olc::FAIL;
		olc_PrepareEngine();
		if (!OnUserCreate()) return olc::FAIL;
		Platform_GLUT::bActiveRef = &bAtomActive;
		glutWMCloseFunc(Platform_GLUT::ExitMainLoop);
		bAtomActive = true;
		platform->StartSystemEventLoop();


		//This code will not even be run but why not
		if (platform->ApplicationCleanUp() != olc::OK) return olc::FAIL;


		return olc::OK;
	}
}


#endif
// O------------------------------------------------------------------------------O
// | END PLATFORM: GLUT                                                           |
// O------------------------------------------------------------------------------O
#pragma endregion
#pragma region emscripten
// O------------------------------------------------------------------------------O
// | START PLATFORM: Emscripten - Totally Game Changing...                        |
// O------------------------------------------------------------------------------O


//
//	Firstly a big mega thank you to members of the OLC Community for sorting this 
//	out. Making a browser compatible version has been a priority for quite some
//	time, but I lacked the expertise to do it. This awesome feature is possible
//	because a group of former strangers got together and formed friendships over
//	their shared passion for code. If anything demonstrates how powerful helping
//	each other can be, it's this. - Javidx9


//	Emscripten Platform: MaGetzUb, Moros1138, Slavka, Dandistine, Gorbit99, Bispoo
//				   also: Ishidex, Gusgo99, SlicEnDicE, Alexio




#if defined(OLC_PLATFORM_EMSCRIPTEN)


#include <emscripten/html5.h>
#include <emscripten/key_codes.h>


extern "C"
{
	EMSCRIPTEN_KEEPALIVE inline int olc_OnPageUnload()
	{
		olc::platform->ApplicationCleanUp(); return 0;
	}
}


namespace olc
{
	export class Platform_Emscripten : public olc::Platform
	{
	public:


		virtual olc::rcode ApplicationStartUp() override
		{
			return olc::rcode::OK;
		}


		virtual olc::rcode ApplicationCleanUp() override
		{
			ThreadCleanUp(); return olc::rcode::OK;
		}


		virtual olc::rcode ThreadStartUp() override
		{
			return olc::rcode::OK;
		}


		virtual olc::rcode ThreadCleanUp() override
		{
			renderer->DestroyDevice(); return olc::OK;
		}


		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) override
		{
			if (renderer->CreateDevice({}, bFullScreen, bEnableVSYNC) == olc::rcode::OK)
			{
				renderer->UpdateViewport(vViewPos, vViewSize);
				return olc::rcode::OK;
			}
			else
				return olc::rcode::FAIL;
		}


		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) override
		{
			emscripten_set_canvas_element_size("#canvas", vWindowSize.x, vWindowSize.y);


			mapKeys[DOM_PK_UNKNOWN] = Key::NONE;
			mapKeys[DOM_PK_A] = Key::A; mapKeys[DOM_PK_B] = Key::B; mapKeys[DOM_PK_C] = Key::C; mapKeys[DOM_PK_D] = Key::D;
			mapKeys[DOM_PK_E] = Key::E; mapKeys[DOM_PK_F] = Key::F; mapKeys[DOM_PK_G] = Key::G; mapKeys[DOM_PK_H] = Key::H;
			mapKeys[DOM_PK_I] = Key::I; mapKeys[DOM_PK_J] = Key::J; mapKeys[DOM_PK_K] = Key::K; mapKeys[DOM_PK_L] = Key::L;
			mapKeys[DOM_PK_M] = Key::M; mapKeys[DOM_PK_N] = Key::N; mapKeys[DOM_PK_O] = Key::O; mapKeys[DOM_PK_P] = Key::P;
			mapKeys[DOM_PK_Q] = Key::Q; mapKeys[DOM_PK_R] = Key::R; mapKeys[DOM_PK_S] = Key::S; mapKeys[DOM_PK_T] = Key::T;
			mapKeys[DOM_PK_U] = Key::U; mapKeys[DOM_PK_V] = Key::V; mapKeys[DOM_PK_W] = Key::W; mapKeys[DOM_PK_X] = Key::X;
			mapKeys[DOM_PK_Y] = Key::Y; mapKeys[DOM_PK_Z] = Key::Z;
			mapKeys[DOM_PK_0] = Key::K0; mapKeys[DOM_PK_1] = Key::K1; mapKeys[DOM_PK_2] = Key::K2;
			mapKeys[DOM_PK_3] = Key::K3; mapKeys[DOM_PK_4] = Key::K4; mapKeys[DOM_PK_5] = Key::K5;
			mapKeys[DOM_PK_6] = Key::K6; mapKeys[DOM_PK_7] = Key::K7; mapKeys[DOM_PK_8] = Key::K8;
			mapKeys[DOM_PK_9] = Key::K9;
			mapKeys[DOM_PK_F1] = Key::F1; mapKeys[DOM_PK_F2] = Key::F2; mapKeys[DOM_PK_F3] = Key::F3; mapKeys[DOM_PK_F4] = Key::F4;
			mapKeys[DOM_PK_F5] = Key::F5; mapKeys[DOM_PK_F6] = Key::F6; mapKeys[DOM_PK_F7] = Key::F7; mapKeys[DOM_PK_F8] = Key::F8;
			mapKeys[DOM_PK_F9] = Key::F9; mapKeys[DOM_PK_F10] = Key::F10; mapKeys[DOM_PK_F11] = Key::F11; mapKeys[DOM_PK_F12] = Key::F12;
			mapKeys[DOM_PK_ARROW_UP] = Key::UP; mapKeys[DOM_PK_ARROW_DOWN] = Key::DOWN;
			mapKeys[DOM_PK_ARROW_LEFT] = Key::LEFT; mapKeys[DOM_PK_ARROW_RIGHT] = Key::RIGHT;
			mapKeys[DOM_PK_SPACE] = Key::SPACE; mapKeys[DOM_PK_TAB] = Key::TAB;
			mapKeys[DOM_PK_SHIFT_LEFT] = Key::SHIFT; mapKeys[DOM_PK_SHIFT_RIGHT] = Key::SHIFT;
			mapKeys[DOM_PK_CONTROL_LEFT] = Key::CTRL; mapKeys[DOM_PK_CONTROL_RIGHT] = Key::CTRL;
			mapKeys[DOM_PK_INSERT] = Key::INS; mapKeys[DOM_PK_DELETE] = Key::DEL; mapKeys[DOM_PK_HOME] = Key::HOME;
			mapKeys[DOM_PK_END] = Key::END; mapKeys[DOM_PK_PAGE_UP] = Key::PGUP; mapKeys[DOM_PK_PAGE_DOWN] = Key::PGDN;
			mapKeys[DOM_PK_BACKSPACE] = Key::BACK; mapKeys[DOM_PK_ESCAPE] = Key::ESCAPE;
			mapKeys[DOM_PK_ENTER] = Key::ENTER; mapKeys[DOM_PK_NUMPAD_EQUAL] = Key::EQUALS;
			mapKeys[DOM_PK_NUMPAD_ENTER] = Key::ENTER; mapKeys[DOM_PK_PAUSE] = Key::PAUSE;
			mapKeys[DOM_PK_SCROLL_LOCK] = Key::SCROLL;
			mapKeys[DOM_PK_NUMPAD_0] = Key::NP0; mapKeys[DOM_PK_NUMPAD_1] = Key::NP1; mapKeys[DOM_PK_NUMPAD_2] = Key::NP2;
			mapKeys[DOM_PK_NUMPAD_3] = Key::NP3; mapKeys[DOM_PK_NUMPAD_4] = Key::NP4; mapKeys[DOM_PK_NUMPAD_5] = Key::NP5;
			mapKeys[DOM_PK_NUMPAD_6] = Key::NP6; mapKeys[DOM_PK_NUMPAD_7] = Key::NP7; mapKeys[DOM_PK_NUMPAD_8] = Key::NP8;
			mapKeys[DOM_PK_NUMPAD_9] = Key::NP9;
			mapKeys[DOM_PK_NUMPAD_MULTIPLY] = Key::NP_MUL; mapKeys[DOM_PK_NUMPAD_DIVIDE] = Key::NP_DIV;
			mapKeys[DOM_PK_NUMPAD_ADD] = Key::NP_ADD; mapKeys[DOM_PK_NUMPAD_SUBTRACT] = Key::NP_SUB;
			mapKeys[DOM_PK_NUMPAD_DECIMAL] = Key::NP_DECIMAL;
			mapKeys[DOM_PK_PERIOD] = Key::PERIOD; mapKeys[DOM_PK_EQUAL] = Key::EQUALS;
			mapKeys[DOM_PK_COMMA] = Key::COMMA; mapKeys[DOM_PK_MINUS] = Key::MINUS;
			mapKeys[DOM_PK_CAPS_LOCK] = Key::CAPS_LOCK;
			mapKeys[DOM_PK_SEMICOLON] = Key::OEM_1;	mapKeys[DOM_PK_SLASH] = Key::OEM_2; mapKeys[DOM_PK_BACKQUOTE] = Key::OEM_3;
			mapKeys[DOM_PK_BRACKET_LEFT] = Key::OEM_4; mapKeys[DOM_PK_BACKSLASH] = Key::OEM_5; mapKeys[DOM_PK_BRACKET_RIGHT] = Key::OEM_6;
			mapKeys[DOM_PK_QUOTE] = Key::OEM_7; mapKeys[DOM_PK_BACKSLASH] = Key::OEM_8;


			// Keyboard Callbacks
			emscripten_set_keydown_callback("#canvas", 0, 1, keyboard_callback);
			emscripten_set_keyup_callback("#canvas", 0, 1, keyboard_callback);


			// Mouse Callbacks
			emscripten_set_wheel_callback("#canvas", 0, 1, wheel_callback);
			emscripten_set_mousedown_callback("#canvas", 0, 1, mouse_callback);
			emscripten_set_mouseup_callback("#canvas", 0, 1, mouse_callback);
			emscripten_set_mousemove_callback("#canvas", 0, 1, mouse_callback);


			// Touch Callbacks
			emscripten_set_touchstart_callback("#canvas", 0, 1, touch_callback);
			emscripten_set_touchmove_callback("#canvas", 0, 1, touch_callback);
			emscripten_set_touchend_callback("#canvas", 0, 1, touch_callback);


			// Canvas Focus Callbacks
			emscripten_set_blur_callback("#canvas", 0, 1, focus_callback);
			emscripten_set_focus_callback("#canvas", 0, 1, focus_callback);

#pragma warning disable format
			EM_ASM(window.onunload = Module._olc_OnPageUnload; );


			// IMPORTANT! - Sorry About This...
			//
			//	In order to handle certain browser based events, such as resizing and
			//	going to full screen, we have to effectively inject code into the container
			//	running the PGE. Yes, I vomited about 11 times too when the others were
			//	convincing me this is the future. Well, this isnt the future, and if it
			//	were to be, I want no part of what must be a miserable distopian free
			//	for all of anarchic code injection to get rudimentary events like "Resize()".
			//
			//	Wake up people! Of course theres a spoon. There has to be to keep feeding
			//	the giant web baby.



			// Fullscreen and Resize Observers
			EM_ASM({


				// cache for reuse
				Module._olc_EmscriptenShellCss = "width: 100%; height: 70vh; margin-left: auto; margin-right: auto;";


			// width / height = aspect ratio
			Module._olc_WindowAspectRatio = $0 / $1;
			Module.canvas.parentNode.addEventListener("resize", function(e) {

				if (e.defaultPrevented) { e.stopPropagation(); 	return; }
				var viewWidth = e.detail.width;
				var viewHeight = e.detail.width / Module._olc_WindowAspectRatio;
				if (viewHeight > e.detail.height)
				{
					viewHeight = e.detail.height;
					viewWidth = e.detail.height * Module._olc_WindowAspectRatio;
				}

				if (Module.canvas.parentNode.className == 'emscripten_border')
					Module.canvas.parentNode.style.cssText = Module._olc_EmscriptenShellCss + " width: " + viewWidth.toString() + "px; height: " + viewHeight.toString() + "px;";


				Module.canvas.setAttribute("width", viewWidth);
				Module.canvas.setAttribute("height", viewHeight);


				if (document.fullscreenElement != null)
				{
					var top = (e.detail.height - viewHeight) / 2;
					var left = (e.detail.width - viewWidth) / 2;
					Module.canvas.style.position = "fixed";
					Module.canvas.style.top = top.toString() + "px";
					Module.canvas.style.left = left.toString() + "px";
					Module.canvas.style.width = "";
					Module.canvas.style.height = "";
				}


				// trigger PGE update
				Module._olc_PGE_UpdateWindowSize(viewWidth, viewHeight);
				// this is really only needed when enter/exiting fullscreen
				Module.canvas.focus();
				// prevent this event from ever affecting the document beyond this element
				e.stopPropagation();
			});


			// helper function to prevent repeating the same code everywhere
			Module._olc_ResizeCanvas = function()
			{
				// yes, we still have to wait, sigh..
				setTimeout(function()
				{
					// if default template, stretch width as well
					if (Module.canvas.parentNode.className == 'emscripten_border')
						Module.canvas.parentNode.style.cssText = Module._olc_EmscriptenShellCss;


					// override it's styling so we can get it's stretched size
					Module.canvas.style.cssText = "width: 100%; height: 100%; outline: none;";


					// setup custom resize event
					var resizeEvent = new CustomEvent('resize',
					{
						detail: {
							width: Module.canvas.clientWidth,
							height : Module.canvas.clientHeight
						},
						bubbles : true,
						cancelable : true
					});


					// trigger custom resize event on canvas element
					Module.canvas.dispatchEvent(resizeEvent);
				}, 50);
			};




			// Disable Refresh Gesture on mobile
			document.body.style.cssText += " overscroll-behavior-y: contain;";


			if (Module.canvas.parentNode.className == 'emscripten_border')
			{
				// force body to have no margin in emscripten's minimal shell
				document.body.style.margin = "0";
				Module.canvas.parentNode.style.cssText = Module._olc_EmscriptenShellCss;
			}

			Module._olc_ResizeCanvas();


			// observe and react to resizing of the container element
			var resizeObserver = new ResizeObserver(function(entries) { Module._olc_ResizeCanvas(); }).observe(Module.canvas.parentNode);


			// observe and react to changes that occur when entering/exiting fullscreen
			var mutationObserver = new MutationObserver(function(mutationsList, observer)
			{
				// a change has occurred, let's check them out!
				for (var i = 0; i < mutationsList.length; i++)
				{
					// cycle through all of the newly added elements
					for (var j = 0; j < mutationsList[i].addedNodes.length; j++)
					{
						// if this element is a our canvas, trigger resize
						if (mutationsList[i].addedNodes[j].id == 'canvas')
							Module._olc_ResizeCanvas();
					}
				}
			}).observe(Module.canvas.parentNode,
			{
				attributes: false,
				childList : true,
				subtree : false
			});


			// add resize listener on window
			window.addEventListener("resize", function(e) { Module._olc_ResizeCanvas(); });


				}, vWindowSize.x, vWindowSize.y); // Fullscreen and Resize Observers
#pragma warning restore format
			return olc::rcode::OK;
		}


		// Interface PGE's UpdateWindowSize, for use in Javascript
		void UpdateWindowSize(int width, int height)
		{
			ptrPGE->olc_UpdateWindowSize(width, height);
		}


		//TY Gorbit
		static EM_BOOL focus_callback(int eventType, const EmscriptenFocusEvent* focusEvent, void* userData)
		{
			if (eventType == EMSCRIPTEN_EVENT_BLUR)
			{
				ptrPGE->olc_UpdateKeyFocus(false);
				ptrPGE->olc_UpdateMouseFocus(false);
			}
			else if (eventType == EMSCRIPTEN_EVENT_FOCUS)
			{
				ptrPGE->olc_UpdateKeyFocus(true);
				ptrPGE->olc_UpdateMouseFocus(true);
			}


			return 0;
		}


		//TY Moros
		static EM_BOOL keyboard_callback(int eventType, const EmscriptenKeyboardEvent* e, void* userData)
		{
			if (eventType == EMSCRIPTEN_EVENT_KEYDOWN)
				ptrPGE->olc_UpdateKeyState(mapKeys[emscripten_compute_dom_pk_code(e->code)], true);


			// THANK GOD!! for this compute function. And thanks Dandistine for pointing it out!
			if (eventType == EMSCRIPTEN_EVENT_KEYUP)
				ptrPGE->olc_UpdateKeyState(mapKeys[emscripten_compute_dom_pk_code(e->code)], false);


			//Consume keyboard events so that keys like F1 and F5 don't do weird things
			return EM_TRUE;
		}


		//TY Moros
		static EM_BOOL wheel_callback(int eventType, const EmscriptenWheelEvent* e, void* userData)
		{
			if (eventType == EMSCRIPTEN_EVENT_WHEEL)
				ptrPGE->olc_UpdateMouseWheel(-1 * e->deltaY);


			return EM_TRUE;
		}


		//TY Bispoo
		static EM_BOOL touch_callback(int eventType, const EmscriptenTouchEvent* e, void* userData)
		{
			// Move
			if (eventType == EMSCRIPTEN_EVENT_TOUCHMOVE)
			{
				ptrPGE->olc_UpdateMouse(e->touches->targetX, e->touches->targetY);
			}


			// Start
			if (eventType == EMSCRIPTEN_EVENT_TOUCHSTART)
			{
				ptrPGE->olc_UpdateMouse(e->touches->targetX, e->touches->targetY);
				ptrPGE->olc_UpdateMouseState(0, true);
			}


			// End
			if (eventType == EMSCRIPTEN_EVENT_TOUCHEND)
			{
				ptrPGE->olc_UpdateMouseState(0, false);
			}


			return EM_TRUE;
		}


		//TY Moros
		static EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData)
		{
			//Mouse Movement
			if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE)
				ptrPGE->olc_UpdateMouse(e->targetX, e->targetY);




			//Mouse button press
			if (e->button == 0) // left click
			{
				if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
					ptrPGE->olc_UpdateMouseState(0, true);
				else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
					ptrPGE->olc_UpdateMouseState(0, false);
			}


			if (e->button == 2) // right click
			{
				if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
					ptrPGE->olc_UpdateMouseState(1, true);
				else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
					ptrPGE->olc_UpdateMouseState(1, false);


			}


			if (e->button == 1) // middle click
			{
				if (eventType == EMSCRIPTEN_EVENT_MOUSEDOWN)
					ptrPGE->olc_UpdateMouseState(2, true);
				else if (eventType == EMSCRIPTEN_EVENT_MOUSEUP)
					ptrPGE->olc_UpdateMouseState(2, false);


				//at the moment only middle mouse needs to consume events.
				return EM_TRUE;
			}


			return EM_FALSE;
		}




		virtual olc::rcode SetWindowTitle(const std::string& s) override
		{
			emscripten_set_window_title(s.c_str()); return olc::OK;
		}


		virtual olc::rcode StartSystemEventLoop() override
		{
			return olc::OK;
		}


		virtual olc::rcode HandleSystemEvent() override
		{
			return olc::OK;
		}


		static void MainLoop()
		{
			olc::Platform::ptrPGE->olc_CoreUpdate();
			if (!ptrPGE->olc_IsRunning())
			{
				if (ptrPGE->OnUserDestroy())
				{
					emscripten_cancel_main_loop();
					platform->ApplicationCleanUp();
				}
				else
				{
					ptrPGE->olc_Reanimate();
				}
			}
		}
	};


	//Emscripten needs a special Start function
	//Much of this is usually done in EngineThread, but that isn't used here
	olc::rcode PixelGameEngine::Start()
	{
		if (platform->ApplicationStartUp() != olc::OK) return olc::FAIL;


		// Construct the window
		if (platform->CreateWindowPane({ 30,30 }, vWindowSize, bFullScreen) != olc::OK) return olc::FAIL;
		olc_UpdateWindowSize(vWindowSize.x, vWindowSize.y);


		// Some implementations may form an event loop here
		if (platform->ThreadStartUp() == olc::FAIL)	return olc::FAIL;


		// Do engine context specific initialisation
		olc_PrepareEngine();


		// Consider the "thread" started
		bAtomActive = true;


		// Create user resources as part of this thread
		for (auto& ext : vExtensions) ext->OnBeforeUserCreate();
		if (!OnUserCreate()) bAtomActive = false;
		for (auto& ext : vExtensions) ext->OnAfterUserCreate();


		platform->StartSystemEventLoop();


		//This causes a heap memory corruption in Emscripten for some reason
		//Platform_Emscripten::bActiveRef = &bAtomActive;
		emscripten_set_main_loop(&Platform_Emscripten::MainLoop, 0, 1);


		// Wait for thread to be exited
		if (platform->ApplicationCleanUp() != olc::OK) return olc::FAIL;
		return olc::OK;
	}
}


extern "C"
{
	EMSCRIPTEN_KEEPALIVE inline void olc_PGE_UpdateWindowSize(int width, int height)
	{
		emscripten_set_canvas_element_size("#canvas", width, height);
		// Thanks slavka
		((olc::Platform_Emscripten*)olc::platform.get())->UpdateWindowSize(width, height);
	}
}


#endif
// O------------------------------------------------------------------------------O
// | END PLATFORM: Emscripten                                                     |
// O------------------------------------------------------------------------------O
#pragma endregion
#pragma endregion