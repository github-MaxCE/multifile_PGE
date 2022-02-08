#pragma once
#include "olcPixelGameEngine.h"
#include "olcPlatform.h"
#include "olcInput.h"

#pragma region platform_glut
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

	class Platform_GLUT : public olc::Platform
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