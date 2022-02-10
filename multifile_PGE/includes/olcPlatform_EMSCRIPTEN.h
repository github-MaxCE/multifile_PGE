#pragma once
#ifndef OLCPLATFORM_EMSCRIPTEN_H
#define OLCPLATFORM_EMSCRIPTEN_H
#include "olcInput.h"
#include "olcPlatform.h"
#include "olcPixelGameEngine.h"

#pragma region platform_emscripten
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
	class Platform_Emscripten : public olc::Platform
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
#endif //OLCPLATFORM_EMSCRIPTEN_H