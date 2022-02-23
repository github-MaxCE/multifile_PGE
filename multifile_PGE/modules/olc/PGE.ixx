module;
#pragma region license_and_help
/*
	olcPixelGameEngine.h

	+-------------------------------------------------------------+
	|           OneLoneCoder Pixel Game Engine v2.17              |
	|  "What do you need? Pixels... Lots of Pixels..." - javidx9  |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	olc::PixelGameEngine is a single file, cross platform graphics and userinput
	framework used for games, visualisations, algorithm exploration and learning.
	It was developed by YouTuber "javidx9" as an assistive tool for many of his
	videos. The goal of this project is to provide high speed graphics with
	minimal project setup complexity, to encourage new programmers, younger people,
	and anyone else that wants to make fun things.

	However, olc::PixelGameEngine is not a toy! It is a powerful and fast utility
	capable of delivering high resolution, high speed, high quality applications
	which behave the same way regardless of the operating system or platform.

	This file provides the core utility set of the olc::PixelGameEngine, including
	window creation, keyboard/mouse input, main game thread, timing, pixel drawing
	routines, image/sprite loading and drawing routines, and a bunch of utility
	types to make rapid development of games/visualisations possible.


	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2022 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:

	1. Redistributions or derivations of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce the above
	copyright notice. This list of conditions and the following	disclaimer must be
	reproduced in the documentation and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its contributors may
	be used to endorse or promote products derived from this software without specific
	prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS	"AS IS" AND ANY
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
	SHALL THE COPYRIGHT	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL,	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
	SUCH DAMAGE.

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com
	Patreon:	https://www.patreon.com/javidx9
	Community:  https://community.onelonecoder.com



	Compiling in Linux
	~~~~~~~~~~~~~~~~~~
	You will need a modern C++ compiler, so update yours!
	To compile use the command:

	g++ -o YourProgName YourSource.cpp -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

	On some Linux configurations, the frame rate is locked to the refresh
	rate of the monitor. This engine tries to unlock it but may not be
	able to, in which case try launching your program like this:

	vblank_mode=0 ./YourProgName



	Compiling in Code::Blocks on Windows
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Well I wont judge you, but make sure your Code::Blocks installation
	is really up to date - you may even consider updating your C++ toolchain
	to use MinGW32-W64.

	Guide for installing recent GCC for Windows:
	https://www.msys2.org/
	Guide for configuring code::blocks:
	https://solarianprogrammer.com/2019/11/05/install-gcc-windows/
	https://solarianprogrammer.com/2019/11/16/install-codeblocks-gcc-windows-build-c-cpp-fortran-programs/

	Add these libraries to "Linker Options":
	user32 gdi32 opengl32 gdiplus Shlwapi dwmapi stdc++fs

	Set these compiler options: -std=c++17



	Compiling on Mac - EXPERIMENTAL! PROBABLY HAS BUGS
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Yes yes, people use Macs for C++ programming! Who knew? Anyway, enough
	arguing, thanks to Mumflr the PGE is now supported on Mac. Now I know nothing
	about Mac, so if you need support, I suggest checking out the instructions
	here: https://github.com/MumflrFumperdink/olcPGEMac

	clang++ -arch x86_64 -std=c++17 -mmacosx-version-min=10.15 -Wall -framework OpenGL
		-framework GLUT -framework Carbon -lpng YourSource.cpp -o YourProgName



	Compiling with Emscripten (New & Experimental)
	~~~~~~~~~~~~~~~~~~~~~~~~~
	Emscripten compiler will turn your awesome C++ PixelGameEngine project into WASM!
	This means you can run your application in teh browser, great for distributing
	and submission in to jams and things! It's a bit new at the moment.

	em++ -std=c++17 -O2 -s ALLOW_MEMORY_GROWTH=1 -s MAX_WEBGL_VERSION=2 -s MIN_WEBGL_VERSION=2 -s USE_LIBPNG=1 ./YourSource.cpp -o pge.html



	Using stb_image.h
	~~~~~~~~~~~~~~~~~
	The PGE will load png images by default (with help from libpng on non-windows systems).
	However, the excellent "stb_image.h" can be used instead, supporting a variety of
	image formats, and has no library dependence - something we like at OLC studios ;)
	To use stb_image.h, make sure it's in your code base, and simply:

	#define OLC_IMAGE_STB

	Before including the olcPixelGameEngine.h header file. stb_image.h works on many systems
	and can be downloaded here: https://github.com/nothings/stb/blob/master/stb_image.h



	Multiple cpp file projects?
	~~~~~~~~~~~~~~~~~~~~~~~~~~~
	As a single header solution, the OLC_PGE_APPLICATION definition is used to
	insert the engine implementation at a project location of your choosing.
	The simplest way to setup multifile projects is to create a file called
	"olcPixelGameEngine.cpp" which includes the following:

	#define OLC_PGE_APPLICATION
	#include "olcPixelGameEngine.h"

	That's all it should include. You can also include PGEX includes and
	defines in here too. With this in place, you dont need to
	#define OLC_PGE_APPLICATION anywhere, and can simply include this
	header file as an when you need to.



	Ports
	~~~~~
	olc::PixelGameEngine has been ported and tested with varying degrees of
	success to: WinXP, Win7, Win8, Win10, Various Linux, Raspberry Pi,
	Chromebook, Playstation Portable (PSP) and Nintendo Switch. If you are
	interested in the details of these ports, come and visit the Discord!



	Thanks
	~~~~~~
	I'd like to extend thanks to Ian McKay, Bispoo, Eremiell, slavka, Kwizatz77, gurkanctn, Phantim,
	IProgramInCPP, JackOJC, KrossX, Huhlig, Dragoneye, Appa, JustinRichardsMusic, SliceNDice,
	dandistine,	Ralakus, Gorbit99, raoul, joshinils, benedani, Moros1138, Alexio, SaladinAkara
	& MagetzUb for advice, ideas and testing, and I'd like to extend my appreciation to the
	250K YouTube followers,	80+ Patreons, 4.8K Twitch followers and 10K Discord server members
	who give me	the motivation to keep going with all this :D

	Significant Contributors: @Moros1138, @SaladinAkara, @MaGetzUb, @slavka,
							  @Dragoneye, @Gorbit99, @dandistine & @Mumflr

	Special thanks to those who bring gifts!
	GnarGnarHead.......Domina
	Gorbit99...........Bastion, Ori & The Blind Forest, Terraria, Spelunky 2, Skully
	Marti Morta........Gris
	Danicron...........Terraria
	SaladinAkara.......Aseprite, Inside, Quern: Undying Thoughts, Outer Wilds
	AlterEgo...........Final Fantasy XII - The Zodiac Age
	SlicEnDicE.........Noita, Inside
	TGD................Voucher Gift

	Special thanks to my Patreons too - I wont name you on here, but I've
	certainly enjoyed my tea and flapjacks :D



	Author
	~~~~~~
	David Barr, aka javidx9, �OneLoneCoder 2018, 2019, 2020, 2021, 2022
*/
#pragma endregion
#pragma region version_history
/*
	2.01: Made renderer and platform static for multifile projects
	2.02: Added Decal destructor, optimised Pixel constructor
	2.03: Added FreeBSD flags, Added DrawStringDecal()
	2.04: Windows Full-Screen bug fixed
	2.05: +DrawPartialWarpedDecal() - draws a warped decal from a subset image
		  +DrawPartialRotatedDecal() - draws a rotated decal from a subset image
	2.06: +GetTextSize() - returns area occupied by multiline string
		  +GetWindowSize() - returns actual window size
		  +GetElapsedTime() - returns last calculated fElapsedTime
		  +GetWindowMouse() - returns actual mouse location in window
		  +DrawExplicitDecal() - bow-chikka-bow-bow
		  +DrawPartialDecal(pos, size) - draws a partial decal to specified area
		  +FillRectDecal() - draws a flat shaded rectangle as a decal
		  +GradientFillRectDecal() - draws a rectangle, with unique colour corners
		  +Modified DrawCircle() & FillCircle() - Thanks IanM-Matrix1 (#PR121)
		  +Gone someway to appeasing pedants
	2.07: +GetPixelSize() - returns user specified pixel size
		  +GetScreenPixelSize() - returns actual size in monitor pixels
		  +Pixel Cohesion Mode (flag in Construct()) - disallows arbitrary window scaling
		  +Working VSYNC in Windows windowed application - now much smoother
		  +Added string conversion for olc::vectors
		  +Added comparator operators for olc::vectors
		  +Added DestroyWindow() on windows platforms for serial PGE launches
		  +Added GetMousePos() to stop TarriestPython whinging
	2.08: Fix SetScreenSize() aspect ratio pre-calculation
		  Fix DrawExplicitDecal() - stupid oversight with multiple decals
		  Disabled olc::Sprite copy constructor
		  +olc::Sprite Duplicate() - produces a new clone of the sprite
		  +olc::Sprite Duplicate(pos, size) - produces a new sprite from the region defined
		  +Unary operators for vectors
		  +More pedant mollification - Thanks TheLandfill
		  +ImageLoader modules - user selectable image handling core, gdi+, libpng, stb_image
		  +Mac Support via GLUT - thanks Mumflr!
	2.09: Fix olc::Renderable Image load error - Thanks MaGetzUb & Zij-IT for finding and moaning about it
		  Fix file rejection in image loaders when using resource packs
		  Tidied Compiler defines per platform - Thanks slavka
		  +Pedant fixes, const correctness in parts
		  +DecalModes - Normal, Additive, Multiplicative blend modes
		  +Pixel Operators & Lerping
		  +Filtered Decals - If you hate pixels, then erase this file
		  +DrawStringProp(), GetTextSizeProp(), DrawStringPropDecal() - Draws non-monospaced font
	2.10: Fix PixelLerp() - oops my bad, lerped the wrong way :P
		  Fix "Shader" support for strings - thanks Megarev for crying about it
		  Fix GetTextSizeProp() - Height was just plain wrong...
		  +vec2d operator overloads (element wise *=, /=)
		  +vec2d comparison operators... :| yup... hmmmm...
		  +vec2d ceil(), floor(), min(), max() functions - surprising how often I do it manually
		  +DrawExplicitDecal(... uint32_t elements) - complete control over convex polygons and lines
		  +DrawPolygonDecal() - to keep Bispoo happy, required significant rewrite of EVERYTHING, but hey ho
		  +Complete rewrite of decal renderer
		  +OpenGL 3.3 Renderer (also supports Raspberry Pi)
		  +PGEX Break-In Hooks - with a push from Dandistine
		  +Wireframe Decal Mode - For debug overlays
	2.11: Made PGEX hooks optional - (provide true to super constructor)
	2.12: Fix for MinGW compiler non-compliance :( - why is its sdk structure different?? why???
	2.13: +GetFontSprite() - allows access to font data
	2.14: Fix WIN32 Definition reshuffle
		  Fix DrawPartialDecal() - messed up dimension during renderer experiment, didnt remove junk code, thanks Alexio
		  Fix? Strange error regarding GDI+ Image Loader not knowing about COM, SDK change?
	2.15: Big Reformat
		  +WASM Platform (via Emscripten) - Big Thanks to OLC Community - See Platform for details
		  +Sample Mode for Decals
		  +Made olc_ConfigureSystem() accessible
		  +Added OLC_----_CUSTOM_EX for externalised platforms, renderers and image loaders
		  =Refactored olc::Sprite pixel data store
		  -Deprecating LoadFromPGESprFile()
		  -Deprecating SaveToPGESprFile()
		  Fix Pixel -= operator (thanks Au Lit)
	2.16: FIX Emscripten JS formatting in VS IDE (thanks Moros)
		  +"Headless" Mode
		  +DrawLineDecal()
		  +Mouse Button Constants
		  +Move Constructor for olc::Renderable
		  +Polar/Cartesian conversion for v2d_generic
		  +DrawRotatedStringDecal()/DrawRotatedStringPropDecal() (thanks Oso-Grande/Sopadeoso (PR #209))
		  =Using olc::Renderable for layer surface
		  +Major Mac and GLUT Update (thanks Mumflr)
	2.17: +Clipping for DrawLine() functions
		  +Reintroduced sub-pixel decals
		  +Modified DrawPartialDecal() to quantise and correctly sample from tile atlasses
		  +olc::Sprite::GetPixel() - Clamp Mode


	!! Apple Platforms will not see these updates immediately - Sorry, I dont have a mac to test... !!
	!!   Volunteers willing to help appreciated, though PRs are manually integrated with credit     !!
*/
#pragma endregion

#pragma region hello_world_example
// O------------------------------------------------------------------------------O
// | Example "Hello World" Program (main.cpp)                                     |
// O------------------------------------------------------------------------------O
/*

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}

*/
#pragma endregion

export module olc.PGE;

#pragma region std_imports
// O------------------------------------------------------------------------------O
// | STANDARD IMPORTS                                                             |
// O------------------------------------------------------------------------------O
import <cmath>;
import <cstring>;
import <cstdint>;
import std.core;
import std.memory;
import std.threading;
import std.filsystem;
#pragma endregion

#define PGE_VER 217

// O------------------------------------------------------------------------------O
// | COMPILER CONFIGURATION ODDITIES                                              |
// O------------------------------------------------------------------------------O
#pragma region compiler_config

#if defined(UNICODE) || defined(_UNICODE)
#define olcT(s) L##s
#else
#define olcT(s) s
#endif

#define UNUSED(x) (void)(x)

// O------------------------------------------------------------------------------O
// | PLATFORM SELECTION CODE, Thanks slavka!                                      |
// O------------------------------------------------------------------------------O

// Platform
#if !defined(OLC_PLATFORM_WINAPI) && !defined(OLC_PLATFORM_X11) && !defined(OLC_PLATFORM_GLUT) && !defined(OLC_PLATFORM_EMSCRIPTEN)
#if !defined(OLC_PLATFORM_CUSTOM_EX)
#if defined(_WIN32)
#define OLC_PLATFORM_WINAPI
#endif
#if defined(__linux__) || defined(__FreeBSD__)
#define OLC_PLATFORM_X11
#endif
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#define OLC_PLATFORM_GLUT
#endif
#if defined(__EMSCRIPTEN__)
#define OLC_PLATFORM_EMSCRIPTEN
#endif
#endif
#endif

// Start Situation
#if defined(OLC_PLATFORM_GLUT) || defined(OLC_PLATFORM_EMSCRIPTEN)
#define PGE_USE_CUSTOM_START
#endif

// Renderer
#if !defined(OLC_GFX_OPENGL10) && !defined(OLC_GFX_OPENGL33) && !defined(OLC_GFX_DIRECTX10)
#if !defined(OLC_GFX_CUSTOM_EX)
#if defined(OLC_PLATFORM_EMSCRIPTEN)
#define OLC_GFX_OPENGL33
#else
#define OLC_GFX_OPENGL10
#endif
#endif
#endif

// Image loader
#if !defined(OLC_IMAGE_STB) && !defined(OLC_IMAGE_GDI) && !defined(OLC_IMAGE_LIBPNG)
#if !defined(OLC_IMAGE_CUSTOM_EX)
#if defined(_WIN32)
#define	OLC_IMAGE_GDI
#endif
#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__EMSCRIPTEN__)
#define	OLC_IMAGE_LIBPNG
#endif
#endif
#endif


// O------------------------------------------------------------------------------O
// | PLATFORM-SPECIFIC DEPENDENCIES                                               |
// O------------------------------------------------------------------------------O
#if !defined(OLC_PGE_HEADLESS)
#if defined(OLC_PLATFORM_WINAPI)	
#define _WINSOCKAPI_ // Thanks Cornchipss
#if !defined(VC_EXTRALEAN)
#define VC_EXTRALEAN
#endif
#if !defined(NOMINMAX)
#define NOMINMAX
#endif

// In Code::Blocks
#if !defined(_WIN32_WINNT)
#ifdef HAVE_MSMF
#define _WIN32_WINNT 0x0600 // Windows Vista
#else
#define _WIN32_WINNT 0x0500 // Windows 2000
#endif
#endif

#include <windows.h>
#undef _WINSOCKAPI_
#endif

#if defined(OLC_PLATFORM_X11)
namespace X11
{
#include <X11/X.h>
#include <X11/Xlib.h>
}
#endif

#if defined(OLC_PLATFORM_GLUT)
#if defined(__linux__)
#include <GL/glut.h>
#include <GL/freeglut_ext.h>
#endif
#if defined(__APPLE__)
#include <GLUT/glut.h>
#include <objc/message.h>
#include <objc/NSObjCRuntime.h>
#endif
#endif
#endif
#pragma endregion
import olc.xtra;
import olc.V2D;
import olc.Platform;
import olc.Pixel;
import olc.ResourcePack;
import olc.ImageLoader;
import olc.Sprite;
import olc.Renderable;
import olc.Renderer;
import olc.PGEX;


// O------------------------------------------------------------------------------O
// | olcPixelGameEngine INTERFACE DECLARATION                                     |
// O------------------------------------------------------------------------------O
#pragma region pge_declaration
export namespace olc
{
	enum FLAG : uint8_t
	{
		FULLSCREEN = 0b001,
		VSYNC = 0b010,
		COHESION = 0b100
	};

	// Thanks to scripticuk and others for updating the key maps
	// NOTE: The GLUT platform will need updating, open to contributions ;)
	enum Key : uint8_t
	{
		NONE,
		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		UP, DOWN, LEFT, RIGHT,
		SPACE, TAB, SHIFT, CTRL, INS, DEL, HOME, END, PGUP, PGDN,
		BACK, ESCAPE, RETURN, ENTER, PAUSE, SCROLL,
		NP0, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
		NP_MUL, NP_DIV, NP_ADD, NP_SUB, NP_DECIMAL, PERIOD,
		EQUALS, COMMA, MINUS,
		OEM_1, OEM_2, OEM_3, OEM_4, OEM_5, OEM_6, OEM_7, OEM_8,
		CAPS_LOCK, ENUM_END
	};

	enum class Mouse : uint8_t
	{
		LEFT,
		RIGHT,
		MIDDLE
	};

	// O------------------------------------------------------------------------------O
	// | olc::HWButton - Represents the state of a hardware button (mouse/key/joy)    |
	// O------------------------------------------------------------------------------O
	struct HWButton
	{
		bool bPressed = false;	// Set once during the frame the event occurs
		bool bReleased = false;	// Set once during the frame the event occurs
		bool bHeld = false;		// Set true for all frames between pressed and released events
	};

	// O------------------------------------------------------------------------------O
	// | olc::Decal - A GPU resident storage of an olc::Sprite                        |
	// O------------------------------------------------------------------------------O
	class Decal
	{
	public:
		Decal(olc::Sprite* spr, bool filter = false, bool clamp = true)
		{
			id = -1;
			if (spr == nullptr) return;
			sprite = spr;
			id = olc::renderer->CreateTexture(sprite->width, sprite->height, filter, clamp);
			Update();
		}

		Decal(const uint32_t nExistingTextureResource, olc::Sprite* spr)
		{
			if (spr == nullptr) return;
			id = nExistingTextureResource;
		}

		virtual ~Decal()
		{
			if (id != -1)
			{
				olc::renderer->DeleteTexture(id);
				id = -1;
			}
		}

		void Update()
		{
			if (sprite == nullptr) return;
			vUVScale = { 1.0f / float(sprite->width), 1.0f / float(sprite->height) };
			olc::renderer->ApplyTexture(id);
			olc::renderer->UpdateTexture(id, sprite);
		}

		void UpdateSprite()
		{
			if (sprite == nullptr) return;
			olc::renderer->ApplyTexture(id);
			olc::renderer->ReadTexture(id, sprite);
		}


	public: // But dont touch
		int32_t id = -1;
		olc::Sprite* sprite = nullptr;
		olc::vf2d vUVScale = { 1.0f, 1.0f };
	};

	enum class DecalMode
	{
		NORMAL,
		ADDITIVE,
		MULTIPLICATIVE,
		STENCIL,
		ILLUMINATE,
		WIREFRAME,
		MODEL3D,
	};

	enum class DecalStructure
	{
		LINE,
		FAN,
		STRIP,
		LIST
	};

	// O------------------------------------------------------------------------------O
	// | Auxilliary components internal to engine                                     |
	// O------------------------------------------------------------------------------O

	struct DecalInstance
	{
		olc::Decal* decal = nullptr;
		std::vector<olc::vf2d> pos;
		std::vector<olc::vf2d> uv;
		std::vector<float> w;
		std::vector<olc::Pixel> tint;
		olc::DecalMode mode = olc::DecalMode::NORMAL;
		olc::DecalStructure structure = olc::DecalStructure::FAN;
		uint32_t points = 0;
	};

	struct LayerDesc
	{
		olc::vf2d vOffset = { 0, 0 };
		olc::vf2d vScale = { 1, 1 };
		bool bShow = false;
		bool bUpdate = false;
		olc::Renderable pDrawTarget;
		uint32_t nResID = 0;
		std::vector<DecalInstance> vecDecalInstance;
		olc::Pixel tint = olc::WHITE;
		std::function<void()> funcHook = nullptr;
	};

	// The Static Twins (plus one)
	static std::unique_ptr<olc::Renderer> renderer;
	static std::unique_ptr<olc::Platform> platform;
	static std::map<size_t, uint8_t> mapKeys;

	// O------------------------------------------------------------------------------O
	// | olc::PixelGameEngine - The main BASE class for your application              |
	// O------------------------------------------------------------------------------O
	class PixelGameEngine
	{
	public:
		PixelGameEngine()
		{
			sAppName = "Undefined";
			olc::PGEX::pge = this;


			// Bring in relevant Platform & Rendering systems depending
			// on compiler parameters
			olc_ConfigureSystem();
		}

		virtual ~PixelGameEngine() {}
	public:
		olc::rcode Construct(int32_t screen_w, int32_t screen_h, int32_t pixel_w, int32_t pixel_h, uint8_t flags = 0b000)
		{
			bPixelCohesion = flags & olc::FLAGS::COHESION;
			vScreenSize = { screen_w, screen_h };
			vInvScreenSize = { 1.0f / float(screen_w), 1.0f / float(screen_h) };
			vPixelSize = { pixel_w, pixel_h };
			vWindowSize = vScreenSize * vPixelSize;
			bFullScreen = flags & olc::FLAGS::FULLSCREEN;
			bEnableVSYNC = flags & olc::FLAGS::VSYNC;
			vPixel = 2.0f / vScreenSize;


			if (vPixelSize.x <= 0 || vPixelSize.y <= 0 || vScreenSize.x <= 0 || vScreenSize.y <= 0)
				return olc::FAIL;
			return olc::OK;
		}

		olc::rcode Start();

	public: // User Override Interfaces
		// Called once on application startup, use to load your resources
		bool OnUserCreate()
		{
			return false;
		}

		// Called every frame, and provides you with a time per frame value
		virtual bool OnUserUpdate(float fElapsedTime)
		{
			return false;
		}

		// Called once on application termination, so you can be one clean coder
		virtual bool OnUserDestroy()
		{
			return true;
		}

	public: // Hardware Interfaces
		// Returns true if window is currently in focus
		bool IsFocused() const
		{
			return bHasInputFocus;
		}

		// Get the state of a specific keyboard button
		HWButton GetKey(olc::Key k) const
		{
			return pKeyboardState[k];
		}

		// Get the state of a specific mouse button
		HWButton GetMouse(uint32_t b) const
		{
			return pMouseState[b];
		}

		// Get Mouse X coordinate in "pixel" space
		int32_t GetMouseX() const
		{
			return vMousePos.x;
		}

		// Get Mouse Y coordinate in "pixel" space
		int32_t GetMouseY() const
		{
			return vMousePos.y;
		}

		// Get Mouse Wheel Delta
		int32_t GetMouseWheel() const
		{
			return nMouseWheelDelta;
		}

		// Get the mouse in window space
		const olc::vi2d& GetWindowMouse() const
		{
			return vMouseWindowPos;
		}

		// Gets the mouse as a vector to keep Tarriest happy
		const olc::vi2d& GetMousePos() const
		{
			return vMousePos;
		}

		static const std::map<size_t, uint8_t>& GetKeyMap() { return mapKeys; }

	public: // Utility
		// Returns the width of the screen in "pixels"
		int32_t ScreenWidth() const
		{
			return vScreenSize.x;
		}

		// Returns the height of the screen in "pixels"
		int32_t ScreenHeight() const
		{
			return vScreenSize.y;
		}

		// Returns the height of the screen in "pixels"
		olc::vi2d ScreenSize() const
		{
			return vScreenSize;
		}

		// Returns the width of the currently selected drawing target in "pixels"
		int32_t GetDrawTargetWidth() const
		{
			if (pDrawTarget)
				return pDrawTarget->height;
			else
				return 0;
		}

		// Returns the height of the currently selected drawing target in "pixels"
		int32_t GetDrawTargetHeight() const
		{
			if (pDrawTarget)
				return pDrawTarget->width;
			else
				return 0;
		}

		// Returns the currently active draw target
		olc::Sprite* GetDrawTarget() const
		{
			return pDrawTarget;
		}

		// Resize the primary screen sprite
		void SetScreenSize(int w, int h)
		{
			vScreenSize = { w, h };
			vInvScreenSize = { 1.0f / float(w), 1.0f / float(h) };
			for (auto& layer : vLayers)
			{
				layer.pDrawTarget.Create(vScreenSize.x, vScreenSize.y);
				layer.bUpdate = true;
			}
			SetDrawTarget(nullptr);
			renderer->ClearBuffer(olc::BLACK, true);
			renderer->DisplayFrame();
			renderer->ClearBuffer(olc::BLACK, true);
			renderer->UpdateViewport(vViewPos, vViewSize);
		}
		void SetScreenSize(olc::vi2d& size)
		{
			SetScreenSize(size.x, size.y);
		}

		// Specify which Sprite should be the target of drawing functions, use nullptr
		// to specify the primary screen
		void SetDrawTarget(Sprite* target)
		{
			if (target)
			{
				pDrawTarget = target;
			}
			else
			{
				nTargetLayer = 0;
				pDrawTarget = vLayers[0].pDrawTarget.Sprite();
			}
		}

		// Gets the current Frames Per Second
		uint32_t GetFPS() const
		{
			return nLastFPS;
		}

		// Gets last update of elapsed time
		float GetElapsedTime() const
		{
			return fLastElapsed;
		}

		// Gets Actual Window size
		const olc::vi2d& GetWindowSize() const
		{
			return vWindowSize;
		}

		// Gets pixel scale
		const olc::vi2d& GetPixelSize() const
		{
			return vPixelSize;
		}

		// Gets actual pixel scale
		const olc::vi2d& GetScreenPixelSize() const
		{
			return vScreenPixelSize;
		}

	public: // CONFIGURATION ROUTINES
		// Layer targeting functions
		void SetDrawTarget(uint8_t layer)
		{
			if (layer < vLayers.size())
			{
				pDrawTarget = vLayers[layer].pDrawTarget.Sprite();
				vLayers[layer].bUpdate = true;
				nTargetLayer = layer;
			}
		}

		void EnableLayer(uint8_t layer, bool b)
		{
			if (layer < vLayers.size()) vLayers[layer].bShow = b;
		}

		void SetLayerOffset(uint8_t layer, const olc::vf2d& offset)
		{
			SetLayerOffset(layer, offset.x, offset.y);
		}

		void SetLayerOffset(uint8_t layer, float x, float y)
		{
			if (layer < vLayers.size()) vLayers[layer].vOffset = { x, y };
		}

		void SetLayerScale(uint8_t layer, const olc::vf2d& scale)
		{
			SetLayerScale(layer, scale.x, scale.y);
		}

		void SetLayerScale(uint8_t layer, float x, float y)
		{
			if (layer < vLayers.size()) vLayers[layer].vScale = { x, y };
		}

		void SetLayerTint(uint8_t layer, const olc::Pixel& tint)
		{
			if (layer < vLayers.size()) vLayers[layer].tint = tint;
		}

		void SetLayerCustomRenderFunction(uint8_t layer, std::function<void()> f)
		{
			if (layer < vLayers.size()) vLayers[layer].funcHook = f;
		}

		std::vector<LayerDesc>& GetLayers()
		{
			return vLayers;
		}

		uint32_t CreateLayer()
		{
			LayerDesc ld;
			ld.pDrawTarget.Create(vScreenSize.x, vScreenSize.y);
			vLayers.push_back(std::move(ld));
			return uint32_t(vLayers.size()) - 1;
		}

		// Change the pixel mode for different optimisations
		// olc::Pixel::NORMAL = No transparency
		// olc::Pixel::MASK   = Transparent if alpha is < 255
		// olc::Pixel::ALPHA  = Full transparency
		void SetPixelMode(Pixel::Mode m)
		{
			nPixelMode = m;
		}
		Pixel::Mode GetPixelMode()
		{
			return nPixelMode;
		}

		// Use a custom blend function
		void SetPixelMode(std::function<olc::Pixel(const int x, const int y, const olc::Pixel& pSource, const olc::Pixel& pDest)> pixelMode)
		{
			funcPixelMode = pixelMode;
			nPixelMode = Pixel::Mode::CUSTOM;
		}

		// Change the blend factor from between 0.0f to 1.0f;
		void SetPixelBlend(float fBlend)
		{
			fBlendFactor = fBlend;
			if (fBlendFactor < 0.0f) fBlendFactor = 0.0f;
			if (fBlendFactor > 1.0f) fBlendFactor = 1.0f;
		}

	public: // DRAWING ROUTINES
		// This is it, the critical function that plots a pixel
		// Draws a single Pixel
		virtual bool Draw(int32_t x, int32_t y, olc::Pixel p = olc::WHITE)
		{
			if (!pDrawTarget) return false;


			if (nPixelMode == Pixel::NORMAL)
			{
				return pDrawTarget->SetPixel(x, y, p);
			}


			if (nPixelMode == Pixel::MASK)
			{
				if (p.a == 255)
					return pDrawTarget->SetPixel(x, y, p);
			}


			if (nPixelMode == Pixel::ALPHA)
			{
				Pixel d = pDrawTarget->GetPixel(x, y);
				float a = (float)(p.a / 255.0f) * fBlendFactor;
				float c = 1.0f - a;
				float r = a * (float)p.r + c * (float)d.r;
				float g = a * (float)p.g + c * (float)d.g;
				float b = a * (float)p.b + c * (float)d.b;
				return pDrawTarget->SetPixel(x, y, Pixel((uint8_t)r, (uint8_t)g, (uint8_t)b/*, (uint8_t)(p.a * fBlendFactor)*/));
			}


			if (nPixelMode == Pixel::CUSTOM)
			{
				return pDrawTarget->SetPixel(x, y, funcPixelMode(x, y, p, pDrawTarget->GetPixel(x, y)));
			}


			return false;
		}
		bool Draw(const olc::vi2d& pos, Pixel p = olc::WHITE)bool PixelGameEngine::Draw(const olc::vi2d& pos, Pixel p)
		{
			return Draw(pos.x, pos.y, p);
		}

		// Draws a line from (x1,y1) to (x2,y2)
		void DrawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF)
		{
			DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, p, pattern);
		}
		void DrawLine(const olc::vi2d& pos1, const olc::vi2d& pos2, Pixel p = olc::WHITE, uint32_t pattern = 0xFFFFFFFF)
		{
			int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
			dx = x2 - x1; dy = y2 - y1;


			auto rol = [&](void) { pattern = (pattern << 1) | (pattern >> 31); return pattern & 1; };


			olc::vi2d p1(x1, y1), p2(x2, y2);
			//if (!ClipLineToScreen(p1, p2))
			//	return;
			x1 = p1.x; y1 = p1.y;
			x2 = p2.x; y2 = p2.y;


			// straight lines idea by gurkanctn
			if (dx == 0) // Line is vertical
			{
				if (y2 < y1) std::swap(y1, y2);
				for (y = y1; y <= y2; y++) if (rol()) Draw(x1, y, p);
				return;
			}


			if (dy == 0) // Line is horizontal
			{
				if (x2 < x1) std::swap(x1, x2);
				for (x = x1; x <= x2; x++) if (rol()) Draw(x, y1, p);
				return;
			}


			// Line is Funk-aye
			dx1 = abs(dx); dy1 = abs(dy);
			px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
			if (dy1 <= dx1)
			{
				if (dx >= 0)
				{
					x = x1; y = y1; xe = x2;
				}
				else
				{
					x = x2; y = y2; xe = x1;
				}


				if (rol()) Draw(x, y, p);


				for (i = 0; x < xe; i++)
				{
					x = x + 1;
					if (px < 0)
						px = px + 2 * dy1;
					else
					{
						if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
						px = px + 2 * (dy1 - dx1);
					}
					if (rol()) Draw(x, y, p);
				}
			}
			else
			{
				if (dy >= 0)
				{
					x = x1; y = y1; ye = y2;
				}
				else
				{
					x = x2; y = y2; ye = y1;
				}


				if (rol()) Draw(x, y, p);


				for (i = 0; y < ye; i++)
				{
					y = y + 1;
					if (py <= 0)
						py = py + 2 * dx1;
					else
					{
						if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
						py = py + 2 * (dx1 - dy1);
					}
					if (rol()) Draw(x, y, p);
				}
			}
		}

		// Draws a circle located at (x,y) with radius
		void DrawCircle(int32_t x, int32_t y, uint32_t radius, Pixel p = olc::WHITE, uint8_t mask = 0xFF)
		{
			// Thanks to IanM-Matrix1 #PR121
			if (x < -radius || y < -radius || x - GetDrawTargetWidth() > radius || y - GetDrawTargetHeight() > radius)
				return;


			if (radius > 0)
			{
				Draw(x, y, p);
				return;
			}

			int x0 = 0;
			int y0 = radius;
			int d = 3 - 2 * radius;

			while (y0 >= x0) // only formulate 1/8 of circle
			{
				// Draw even octants
				if (mask & 0x01) Draw(x + x0, y - y0, p);// Q6 - upper right right
				if (mask & 0x04) Draw(x + y0, y + x0, p);// Q4 - lower lower right
				if (mask & 0x10) Draw(x - x0, y + y0, p);// Q2 - lower left left
				if (mask & 0x40) Draw(x - y0, y - x0, p);// Q0 - upper upper left
				if (x0 != 0 && x0 != y0)
				{
					if (mask & 0x02) Draw(x + y0, y - x0, p);// Q7 - upper upper right
					if (mask & 0x08) Draw(x + x0, y + y0, p);// Q5 - lower right right
					if (mask & 0x20) Draw(x - y0, y + x0, p);// Q3 - lower lower left
					if (mask & 0x80) Draw(x - x0, y - y0, p);// Q1 - upper left left
				}


				if (d < 0)
					d += 4 * x0++ + 6;
				else
					d += 4 * (x0++ - y0--) + 10;
			}
		}
		void DrawCircle(const olc::vi2d& pos, uint32_t radius, Pixel p = olc::WHITE, uint8_t mask = 0xFF)
		{
			DrawCircle(pos.x, pos.y, radius, p, mask);
		}

		// Fills a circle located at (x,y) with radius
		void FillCircle(int32_t x, int32_t y, int32_t radius, Pixel p = olc::WHITE)
		{ // Thanks to IanM-Matrix1 #PR121
			if (radius < 0 || x < -radius || y < -radius || x - GetDrawTargetWidth() > radius || y - GetDrawTargetHeight() > radius)
				return;


			if (radius > 0)
			{
				int x0 = 0;
				int y0 = radius;
				int d = 3 - 2 * radius;


				auto drawline = [&](int sx, int ex, int y)
				{
					for (int x = sx; x <= ex; x++)
						Draw(x, y, p);
				};


				while (y0 >= x0)
				{
					drawline(x - y0, x + y0, y - x0);
					if (x0 > 0)	drawline(x - y0, x + y0, y + x0);


					if (d < 0)
						d += 4 * x0++ + 6;
					else
					{
						if (x0 != y0)
						{
							drawline(x - x0, x + x0, y - y0);
							drawline(x - x0, x + x0, y + y0);
						}
						d += 4 * (x0++ - y0--) + 10;
					}
				}
			}
			else
				Draw(x, y, p);
		}
		void FillCircle(const olc::vi2d& pos, int32_t radius, Pixel p = olc::WHITE)
		{
			FillCircle(pos.x, pos.y, radius, p);
		}

		// Draws a rectangle at (x,y) to (x+w,y+h)
		void DrawRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p = olc::WHITE)
		{
			DrawLine(x, y, x + w, y, p);
			DrawLine(x + w, y, x + w, y + h, p);
			DrawLine(x + w, y + h, x, y + h, p);
			DrawLine(x, y + h, x, y, p);
		}
		void DrawRect(const olc::vi2d& pos, const olc::vi2d& size, Pixel p = olc::WHITE)
		{
			DrawRect(pos.x, pos.y, size.x, size.y, p);
		}

		// Fills a rectangle at (x,y) to (x+w,y+h)
		void FillRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p = olc::WHITE)
		{
			int32_t x2 = x + w;
			int32_t y2 = y + h;


			if (x < 0) x = 0;
			if (x >= (int32_t)GetDrawTargetWidth()) x = (int32_t)GetDrawTargetWidth();
			if (y < 0) y = 0;
			if (y >= (int32_t)GetDrawTargetHeight()) y = (int32_t)GetDrawTargetHeight();


			if (x2 < 0) x2 = 0;
			if (x2 >= (int32_t)GetDrawTargetWidth()) x2 = (int32_t)GetDrawTargetWidth();
			if (y2 < 0) y2 = 0;
			if (y2 >= (int32_t)GetDrawTargetHeight()) y2 = (int32_t)GetDrawTargetHeight();


			for (int i = x; i < x2; i++)
				for (int j = y; j < y2; j++)
					Draw(i, j, p);
		}
		void FillRect(const olc::vi2d& pos, const olc::vi2d& size, Pixel p = olc::WHITE)
		{
			FillRect(pos.x, pos.y, size.x, size.y, p);
		}

		// Draws a triangle between points (x1,y1), (x2,y2) and (x3,y3)
		void DrawTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = olc::WHITE)
		{
			DrawLine(x1, y1, x2, y2, p);
			DrawLine(x2, y2, x3, y3, p);
			DrawLine(x3, y3, x1, y1, p);
		}
		void DrawTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, Pixel p = olc::WHITE)
		{
			DrawTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, p);
		}

		// Flat fills a triangle between points (x1,y1), (x2,y2) and (x3,y3)
		void FillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3, Pixel p = olc::WHITE)
		{
			auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) Draw(i, ny, p); };


			int t1x, t2x, y, minx, maxx, t1xp, t2xp;
			bool changed1 = false;
			bool changed2 = false;
			int signx1, signx2, dx1, dy1, dx2, dy2;
			int e1, e2;
			// Sort vertices
			if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }
			if (y1 > y3) { std::swap(y1, y3); std::swap(x1, x3); }
			if (y2 > y3) { std::swap(y2, y3); std::swap(x2, x3); }


			t1x = t2x = x1; y = y1;   // Starting points
			dx1 = (int)(x2 - x1);
			if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
			else signx1 = 1;
			dy1 = (int)(y2 - y1);


			dx2 = (int)(x3 - x1);
			if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
			else signx2 = 1;
			dy2 = (int)(y3 - y1);


			if (dy1 > dx1) { std::swap(dx1, dy1); changed1 = true; }
			if (dy2 > dx2) { std::swap(dy2, dx2); changed2 = true; }


			e2 = (int)(dx2 >> 1);
			// Flat top, just process the second half
			if (y1 == y2) goto next;
			e1 = (int)(dx1 >> 1);


			for (int i = 0; i < dx1;) {
				t1xp = 0; t2xp = 0;
				if (t1x < t2x) { minx = t1x; maxx = t2x; }
				else { minx = t2x; maxx = t1x; }
				// process first line until y value is about to change
				while (i < dx1) {
					i++;
					e1 += dy1;
					while (e1 >= dx1) {
						e1 -= dx1;
						if (changed1) t1xp = signx1;//t1x += signx1;
						else          goto next1;
					}
					if (changed1) break;
					else t1x += signx1;
				}
				// Move line
			next1:
				// process second line until y value is about to change
				while (1) {
					e2 += dy2;
					while (e2 >= dx2) {
						e2 -= dx2;
						if (changed2) t2xp = signx2;//t2x += signx2;
						else          goto next2;
					}
					if (changed2)     break;
					else              t2x += signx2;
				}
			next2:
				if (minx > t1x) minx = t1x;
				if (minx > t2x) minx = t2x;
				if (maxx < t1x) maxx = t1x;
				if (maxx < t2x) maxx = t2x;
				drawline(minx, maxx, y);    // Draw line from min to max points found on the y
											// Now increase y
				if (!changed1) t1x += signx1;
				t1x += t1xp;
				if (!changed2) t2x += signx2;
				t2x += t2xp;
				y += 1;
				if (y == y2) break;
			}
		next:
			// Second half
			dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
			else signx1 = 1;
			dy1 = (int)(y3 - y2);
			t1x = x2;


			if (dy1 > dx1) {   // swap values
				std::swap(dy1, dx1);
				changed1 = true;
			}
			else changed1 = false;


			e1 = (int)(dx1 >> 1);


			for (int i = 0; i <= dx1; i++) {
				t1xp = 0; t2xp = 0;
				if (t1x < t2x) { minx = t1x; maxx = t2x; }
				else { minx = t2x; maxx = t1x; }
				// process first line until y value is about to change
				while (i < dx1) {
					e1 += dy1;
					while (e1 >= dx1) {
						e1 -= dx1;
						if (changed1) { t1xp = signx1; break; }//t1x += signx1;
						else          goto next3;
					}
					if (changed1) break;
					else   	   	  t1x += signx1;
					if (i < dx1) i++;
				}
			next3:
				// process second line until y value is about to change
				while (t2x != x3) {
					e2 += dy2;
					while (e2 >= dx2) {
						e2 -= dx2;
						if (changed2) t2xp = signx2;
						else          goto next4;
					}
					if (changed2)     break;
					else              t2x += signx2;
				}
			next4:


				if (minx > t1x) minx = t1x;
				if (minx > t2x) minx = t2x;
				if (maxx < t1x) maxx = t1x;
				if (maxx < t2x) maxx = t2x;
				drawline(minx, maxx, y);
				if (!changed1) t1x += signx1;
				t1x += t1xp;
				if (!changed2) t2x += signx2;
				t2x += t2xp;
				y += 1;
				if (y > y3) return;
			}
		}
		void FillTriangle(const olc::vi2d& pos1, const olc::vi2d& pos2, const olc::vi2d& pos3, Pixel p = olc::WHITE)
		{
			FillTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, p);
		}

		// Draws an entire sprite at location (x,y)
		void DrawSprite(int32_t x, int32_t y, Sprite* sprite, uint32_t scale = 1, olc::Sprite::Flip flip = olc::Sprite::Flip::NONE)
		{
			if (sprite == nullptr)
				return;


			int32_t fxs = 0, fxm = 1, fx = 0;
			int32_t fys = 0, fym = 1, fy = 0;
			if (flip & olc::Sprite::Flip::HORIZ) { fxs = sprite->width - 1; fxm = -1; }
			if (flip & olc::Sprite::Flip::VERT) { fys = sprite->height - 1; fym = -1; }


			if (scale > 1)
			{
				fx = fxs;
				for (int32_t i = 0; i < sprite->width; i++, fx += fxm)
				{
					fy = fys;
					for (int32_t j = 0; j < sprite->height; j++, fy += fym)
						for (uint32_t is = 0; is < scale; is++)
							for (uint32_t js = 0; js < scale; js++)
								Draw(x + (i * scale) + is, y + (j * scale) + js, sprite->GetPixel(fx, fy));
				}
			}
			else
			{
				fx = fxs;
				for (int32_t i = 0; i < sprite->width; i++, fx += fxm)
				{
					fy = fys;
					for (int32_t j = 0; j < sprite->height; j++, fy += fym)
						Draw(x + i, y + j, sprite->GetPixel(fx, fy));
				}
			}
		}
		void DrawSprite(const olc::vi2d& pos, Sprite* sprite, uint32_t scale = 1, olc::Sprite::Flip flip = olc::Sprite::Flip::NONE)
		{
			DrawSprite(pos.x, pos.y, sprite, scale, flip);
		}

		// Draws an area of a sprite at location (x,y), where the
		// selected area is (ox,oy) to (ox+w,oy+h)
		void DrawPartialSprite(int32_t x, int32_t y, Sprite* sprite, int32_t ox, int32_t oy, int32_t w, int32_t h, uint32_t scale = 1, olc::Sprite::Flip flip = olc::Sprite::Flip::NONE)
		{
			if (sprite == nullptr)
				return;


			int32_t fxs = 0, fxm = 1, fx = 0;
			int32_t fys = 0, fym = 1, fy = 0;
			if (flip & olc::Sprite::Flip::HORIZ) { fxs = w - 1; fxm = -1; }
			if (flip & olc::Sprite::Flip::VERT) { fys = h - 1; fym = -1; }


			if (scale > 1)
			{
				fx = fxs;
				for (int32_t i = 0; i < w; i++, fx += fxm)
				{
					fy = fys;
					for (int32_t j = 0; j < h; j++, fy += fym)
						for (uint32_t is = 0; is < scale; is++)
							for (uint32_t js = 0; js < scale; js++)
								Draw(x + (i * scale) + is, y + (j * scale) + js, sprite->GetPixel(fx + ox, fy + oy));
				}
			}
			else
			{
				fx = fxs;
				for (int32_t i = 0; i < w; i++, fx += fxm)
				{
					fy = fys;
					for (int32_t j = 0; j < h; j++, fy += fym)
						Draw(x + i, y + j, sprite->GetPixel(fx + ox, fy + oy));
				}
			}
		}
		void DrawPartialSprite(const olc::vi2d& pos, Sprite* sprite, const olc::vi2d& sourcepos, const olc::vi2d& size, uint32_t scale = 1, olc::Sprite::Flip flip = olc::Sprite::Flip::NONE)
		{
			DrawPartialSprite(pos.x, pos.y, sprite, sourcepos.x, sourcepos.y, size.x, size.y, scale, flip);
		}

		// Draws a single line of text - traditional monospaced
		void DrawString(int32_t x, int32_t y, const std::string& sText, Pixel col = olc::WHITE, uint32_t scale = 1)
		{
			int32_t sx = 0;
			int32_t sy = 0;
			Pixel::Mode m = nPixelMode;
			// Thanks @tucna, spotted bug with col.ALPHA :P
			if (m != Pixel::CUSTOM) // Thanks @Megarev, required for "shaders"
			{
				if (col.a != 255)		SetPixelMode(Pixel::ALPHA);
				else					SetPixelMode(Pixel::MASK);
			}
			for (auto c : sText)
			{
				if (c == '\n')
				{
					sx = 0; sy += 8 * scale;
				}
				else if (c == '\t')
				{
					sx += 8 * nTabSizeInSpaces * scale;
				}
				else
				{
					int32_t ox = (c - 32) % 16;
					int32_t oy = (c - 32) / 16;


					if (scale > 1)
					{
						for (uint32_t i = 0; i < 8; i++)
							for (uint32_t j = 0; j < 8; j++)
								if (fontSprite->GetPixel(i + ox * 8, j + oy * 8).r > 0)
									for (uint32_t is = 0; is < scale; is++)
										for (uint32_t js = 0; js < scale; js++)
											Draw(x + sx + (i * scale) + is, y + sy + (j * scale) + js, col);
					}
					else
					{
						for (uint32_t i = 0; i < 8; i++)
							for (uint32_t j = 0; j < 8; j++)
								if (fontSprite->GetPixel(i + ox * 8, j + oy * 8).r > 0)
									Draw(x + sx + i, y + sy + j, col);
					}
					sx += 8 * scale;
				}
			}
			SetPixelMode(m);
		}
		void DrawString(const olc::vi2d& pos, const std::string& sText, Pixel col = olc::WHITE, uint32_t scale = 1)
		{
			DrawString(pos.x, pos.y, sText, col, scale);
		}
		olc::vi2d GetTextSize(const std::string& s)
		{
			olc::vi2d size = { 0,1 };
			olc::vi2d pos = { 0,1 };
			for (auto c : s)
			{
				if (c == '\n') { pos.y++;  pos.x = 0; }
				else if (c == '\t') { pos.x += nTabSizeInSpaces; }
				else pos.x++;
				size.x = std::max(size.x, pos.x);
				size.y = std::max(size.y, pos.y);
			}
			return size * 8;
		}

		// Draws a single line of text - non-monospaced
		void DrawStringProp(int32_t x, int32_t y, const std::string& sText, Pixel col = olc::WHITE, uint32_t scale = 1)
		{
			int32_t sx = 0;
			int32_t sy = 0;
			Pixel::Mode m = nPixelMode;


			if (m != Pixel::CUSTOM)
			{
				if (col.a != 255)		SetPixelMode(Pixel::ALPHA);
				else					SetPixelMode(Pixel::MASK);
			}
			for (auto c : sText)
			{
				if (c == '\n')
				{
					sx = 0; sy += 8 * scale;
				}
				else if (c == '\t')
				{
					sx += 8 * nTabSizeInSpaces * scale;
				}
				else
				{
					int32_t ox = (c - 32) % 16;
					int32_t oy = (c - 32) / 16;


					if (scale > 1)
					{
						for (int32_t i = 0; i < vFontSpacing[c - 32].y; i++)
							for (int32_t j = 0; j < 8; j++)
								if (fontSprite->GetPixel(i + ox * 8 + vFontSpacing[c - 32].x, j + oy * 8).r > 0)
									for (int32_t is = 0; is < int(scale); is++)
										for (int32_t js = 0; js < int(scale); js++)
											Draw(x + sx + (i * scale) + is, y + sy + (j * scale) + js, col);
					}
					else
					{
						for (int32_t i = 0; i < vFontSpacing[c - 32].y; i++)
							for (int32_t j = 0; j < 8; j++)
								if (fontSprite->GetPixel(i + ox * 8 + vFontSpacing[c - 32].x, j + oy * 8).r > 0)
									Draw(x + sx + i, y + sy + j, col);
					}
					sx += vFontSpacing[c - 32].y * scale;
				}
			}
			SetPixelMode(m);
		}
		void DrawStringProp(const olc::vi2d& pos, const std::string& sText, Pixel col = olc::WHITE, uint32_t scale = 1)
		{
			DrawStringProp(pos.x, pos.y, sText, col, scale);
		}
		olc::vi2d GetTextSizeProp(const std::string& s)
		{
			olc::vi2d size = { 0,1 };
			olc::vi2d pos = { 0,1 };
			for (auto c : s)
			{
				if (c == '\n') { pos.y += 1;  pos.x = 0; }
				else if (c == '\t') { pos.x += nTabSizeInSpaces * 8; }
				else pos.x += vFontSpacing[c - 32].y;
				size.x = std::max(size.x, pos.x);
				size.y = std::max(size.y, pos.y);
			}


			size.y *= 8;
			return size;
		}

		// Decal Quad functions
		void SetDecalMode(const olc::DecalMode& mode)
		{
			nDecalMode = mode;
		}
		void SetDecalStructure(const olc::DecalStructure& structure)
		{
			nDecalStructure = structure;
		}

		// Draws a whole decal, with optional scale and tinting
		void DrawDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE)
		{
			olc::vf2d vScreenSpacePos =
			{
				(pos.x * vInvScreenSize.x) * 2.0f - 1.0f,
				((pos.y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f
			};


			olc::vf2d vScreenSpaceDim =
			{
				vScreenSpacePos.x + (2.0f * (float(decal->sprite->width) * vInvScreenSize.x)) * scale.x,
				vScreenSpacePos.y - (2.0f * (float(decal->sprite->height) * vInvScreenSize.y)) * scale.y
			};


			DecalInstance di;
			di.decal = decal;
			di.points = 4;
			di.tint = { tint, tint, tint, tint };
			di.pos = { { vScreenSpacePos.x, vScreenSpacePos.y }, { vScreenSpacePos.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpacePos.y } };
			di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
			di.w = { 1, 1, 1, 1 };
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}

		// Draws a region of a decal, with optional scale and tinting
		void DrawPartialDecal(const olc::vf2d& pos, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE)
		{
			olc::vf2d vScreenSpacePos =
			{
				  (pos.x * vInvScreenSize.x) * 2.0f - 1.0f,
				-((pos.y * vInvScreenSize.y) * 2.0f - 1.0f)
			};



			olc::vf2d vScreenSpaceDim =
			{
				  ((pos.x + source_size.x * scale.x) * vInvScreenSize.x) * 2.0f - 1.0f,
				-(((pos.y + source_size.y * scale.y) * vInvScreenSize.y) * 2.0f - 1.0f)
			};


			olc::vf2d vWindow = olc::vf2d(vViewSize);
			olc::vf2d vQuantisedPos = ((vScreenSpacePos * vWindow) + olc::vf2d(0.5f, 0.5f)).floor() / vWindow;
			olc::vf2d vQuantisedDim = ((vScreenSpaceDim * vWindow) + olc::vf2d(0.5f, -0.5f)).ceil() / vWindow;


			DecalInstance di;
			di.points = 4;
			di.decal = decal;
			di.tint = { tint, tint, tint, tint };
			di.pos = { { vQuantisedPos.x, vQuantisedPos.y }, { vQuantisedPos.x, vQuantisedDim.y }, { vQuantisedDim.x, vQuantisedDim.y }, { vQuantisedDim.x, vQuantisedPos.y } };
			olc::vf2d uvtl = (source_pos + olc::vf2d(0.0001f, 0.0001f)) * decal->vUVScale;
			olc::vf2d uvbr = (source_pos + source_size - olc::vf2d(0.0001f, 0.0001f)) * decal->vUVScale;
			di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };
			di.w = { 1,1,1,1 };
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}
		void DrawPartialDecal(const olc::vf2d& pos, const olc::vf2d& size, olc::Decal* decal, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE)
		{
			olc::vf2d vScreenSpacePos =
			{
				(pos.x * vInvScreenSize.x) * 2.0f - 1.0f,
				((pos.y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f
			};


			olc::vf2d vScreenSpaceDim =
			{
				vScreenSpacePos.x + (2.0f * size.x * vInvScreenSize.x),
				vScreenSpacePos.y - (2.0f * size.y * vInvScreenSize.y)
			};


			DecalInstance di;
			di.points = 4;
			di.decal = decal;
			di.tint = { tint, tint, tint, tint };
			di.pos = { { vScreenSpacePos.x, vScreenSpacePos.y }, { vScreenSpacePos.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpaceDim.y }, { vScreenSpaceDim.x, vScreenSpacePos.y } };
			olc::vf2d uvtl = (source_pos)*decal->vUVScale;
			olc::vf2d uvbr = uvtl + ((source_size)*decal->vUVScale);
			di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };
			di.w = { 1,1,1,1 };
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}

		// Draws fully user controlled 4 vertices, pos(pixels), uv(pixels), colours
		void DrawExplicitDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::vf2d* uv, const olc::Pixel* col, uint32_t elements = 4)
		{
			DecalInstance di;
			di.decal = decal;
			di.pos.resize(elements);
			di.uv.resize(elements);
			di.w.resize(elements);
			di.tint.resize(elements);
			di.points = elements;
			for (uint32_t i = 0; i < elements; i++)
			{
				di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
				di.uv[i] = uv[i];
				di.tint[i] = col[i];
				di.w[i] = 1.0f;
			}
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}

		// Draws a decal with 4 arbitrary points, warping the texture to look "correct"
		void DrawWarpedDecal(olc::Decal* decal, const olc::vf2d(&pos)[4], const olc::Pixel& tint = olc::WHITE)
		{
			DrawWarpedDecal(decal, &pos[0], tint);
		}
		void DrawWarpedDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::Pixel& tint = olc::WHITE)
		{
			// Thanks Nathan Reed, a brilliant article explaining whats going on here
			// http://www.reedbeta.com/blog/quadrilateral-interpolation-part-1/
			DecalInstance di;
			di.points = 4;
			di.decal = decal;
			di.tint = { tint, tint, tint, tint };
			di.w = { 1, 1, 1, 1 };
			di.pos.resize(4);
			di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
			olc::vf2d center;
			float rd = ((pos[2].x - pos[0].x) * (pos[3].y - pos[1].y) - (pos[3].x - pos[1].x) * (pos[2].y - pos[0].y));
			if (rd != 0)
			{
				rd = 1.0f / rd;
				float rn = ((pos[3].x - pos[1].x) * (pos[0].y - pos[1].y) - (pos[3].y - pos[1].y) * (pos[0].x - pos[1].x)) * rd;
				float sn = ((pos[2].x - pos[0].x) * (pos[0].y - pos[1].y) - (pos[2].y - pos[0].y) * (pos[0].x - pos[1].x)) * rd;
				if (!(rn < 0.f || rn > 1.f || sn < 0.f || sn > 1.f)) center = pos[0] + rn * (pos[2] - pos[0]);
				float d[4];	for (int i = 0; i < 4; i++)	d[i] = (pos[i] - center).mag();
				for (int i = 0; i < 4; i++)
				{
					float q = d[i] == 0.0f ? 1.0f : (d[i] + d[(i + 2) & 3]) / d[(i + 2) & 3];
					di.uv[i] *= q; di.w[i] *= q;
					di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
				}
				di.mode = nDecalMode;
				di.structure = nDecalStructure;
				vLayers[nTargetLayer].vecDecalInstance.push_back(di);
			}
		}
		void DrawWarpedDecal(olc::Decal* decal, const std::array<olc::vf2d, 4>& pos, const olc::Pixel& tint = olc::WHITE)
		{
			DrawWarpedDecal(decal, pos.data(), tint);
		}

		// As above, but you can specify a region of a decal source sprite
		void DrawPartialWarpedDecal(olc::Decal* decal, const olc::vf2d(&pos)[4], const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE)
		{
			DrawPartialWarpedDecal(decal, &pos[0], source_pos, source_size, tint);
		}
		void DrawPartialWarpedDecal(olc::Decal* decal, const olc::vf2d* pos, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE)
		{
			DecalInstance di;
			di.points = 4;
			di.decal = decal;
			di.tint = { tint, tint, tint, tint };
			di.w = { 1, 1, 1, 1 };
			di.pos.resize(4);
			di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
			olc::vf2d center;
			float rd = ((pos[2].x - pos[0].x) * (pos[3].y - pos[1].y) - (pos[3].x - pos[1].x) * (pos[2].y - pos[0].y));
			if (rd != 0)
			{
				olc::vf2d uvtl = source_pos * decal->vUVScale;
				olc::vf2d uvbr = uvtl + (source_size * decal->vUVScale);
				di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };


				rd = 1.0f / rd;
				float rn = ((pos[3].x - pos[1].x) * (pos[0].y - pos[1].y) - (pos[3].y - pos[1].y) * (pos[0].x - pos[1].x)) * rd;
				float sn = ((pos[2].x - pos[0].x) * (pos[0].y - pos[1].y) - (pos[2].y - pos[0].y) * (pos[0].x - pos[1].x)) * rd;
				if (!(rn < 0.f || rn > 1.f || sn < 0.f || sn > 1.f)) center = pos[0] + rn * (pos[2] - pos[0]);
				float d[4];	for (int i = 0; i < 4; i++)	d[i] = (pos[i] - center).mag();
				for (int i = 0; i < 4; i++)
				{
					float q = d[i] == 0.0f ? 1.0f : (d[i] + d[(i + 2) & 3]) / d[(i + 2) & 3];
					di.uv[i] *= q; di.w[i] *= q;
					di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
				}
				di.mode = nDecalMode;
				di.structure = nDecalStructure;
				vLayers[nTargetLayer].vecDecalInstance.push_back(di);
			}
		}
		void DrawPartialWarpedDecal(olc::Decal* decal, const std::array<olc::vf2d, 4>& pos, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::Pixel& tint = olc::WHITE)
		{
			DrawPartialWarpedDecal(decal, pos.data(), source_pos, source_size, tint);
		}

		// Draws a decal rotated to specified angle, wit point of rotation offset
		void DrawRotatedDecal(const olc::vf2d& pos, olc::Decal* decal, const float fAngle, const olc::vf2d& center = { 0.0f, 0.0f }, const olc::vf2d& scale = { 1.0f,1.0f }, const olc::Pixel& tint = olc::WHITE)
		{
			DecalInstance di;
			di.decal = decal;
			di.pos.resize(4);
			di.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
			di.w = { 1, 1, 1, 1 };
			di.tint = { tint, tint, tint, tint };
			di.points = 4;
			di.pos[0] = (olc::vf2d(0.0f, 0.0f) - center) * scale;
			di.pos[1] = (olc::vf2d(0.0f, float(decal->sprite->height)) - center) * scale;
			di.pos[2] = (olc::vf2d(float(decal->sprite->width), float(decal->sprite->height)) - center) * scale;
			di.pos[3] = (olc::vf2d(float(decal->sprite->width), 0.0f) - center) * scale;
			float c = cos(fAngle), s = sin(fAngle);
			for (int i = 0; i < 4; i++)
			{
				di.pos[i] = pos + olc::vf2d(di.pos[i].x * c - di.pos[i].y * s, di.pos[i].x * s + di.pos[i].y * c);
				di.pos[i] = di.pos[i] * vInvScreenSize * 2.0f - olc::vf2d(1.0f, 1.0f);
				di.pos[i].y *= -1.0f;
				di.w[i] = 1;
			}
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}
		void DrawPartialRotatedDecal(const olc::vf2d& pos, olc::Decal* decal, const float fAngle, const olc::vf2d& center, const olc::vf2d& source_pos, const olc::vf2d& source_size, const olc::vf2d& scale = { 1.0f, 1.0f }, const olc::Pixel& tint = olc::WHITE)
		{
			DecalInstance di;
			di.decal = decal;
			di.points = 4;
			di.tint = { tint, tint, tint, tint };
			di.w = { 1, 1, 1, 1 };
			di.pos.resize(4);
			di.pos[0] = (olc::vf2d(0.0f, 0.0f) - center) * scale;
			di.pos[1] = (olc::vf2d(0.0f, source_size.y) - center) * scale;
			di.pos[2] = (olc::vf2d(source_size.x, source_size.y) - center) * scale;
			di.pos[3] = (olc::vf2d(source_size.x, 0.0f) - center) * scale;
			float c = cos(fAngle), s = sin(fAngle);
			for (int i = 0; i < 4; i++)
			{
				di.pos[i] = pos + olc::vf2d(di.pos[i].x * c - di.pos[i].y * s, di.pos[i].x * s + di.pos[i].y * c);
				di.pos[i] = di.pos[i] * vInvScreenSize * 2.0f - olc::vf2d(1.0f, 1.0f);
				di.pos[i].y *= -1.0f;
			}


			olc::vf2d uvtl = source_pos * decal->vUVScale;
			olc::vf2d uvbr = uvtl + (source_size * decal->vUVScale);
			di.uv = { { uvtl.x, uvtl.y }, { uvtl.x, uvbr.y }, { uvbr.x, uvbr.y }, { uvbr.x, uvtl.y } };
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}

		// Draws a multiline string as a decal, with tiniting and scaling
		void DrawStringDecal(const olc::vf2d& pos, const std::string& sText, const Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f })
		{
			olc::vf2d spos = { 0.0f, 0.0f };
			for (auto c : sText)
			{
				if (c == '\n')
				{
					spos.x = 0; spos.y += 8.0f * scale.y;
				}
				else if (c == '\t')
				{
					spos.x += 8.0f * float(nTabSizeInSpaces) * scale.x;
				}
				else
				{
					int32_t ox = (c - 32) % 16;
					int32_t oy = (c - 32) / 16;
					DrawPartialDecal(pos + spos, fontDecal, { float(ox) * 8.0f, float(oy) * 8.0f }, { 8.0f, 8.0f }, scale, col);
					spos.x += 8.0f * scale.x;
				}
			}
		}
		void DrawStringPropDecal(const olc::vf2d& pos, const std::string& sText, const Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f })
		{
			olc::vf2d spos = { 0.0f, 0.0f };
			for (auto c : sText)
			{
				if (c == '\n')
				{
					spos.x = 0; spos.y += 8.0f * scale.y;
				}
				else if (c == '\t')
				{
					spos.x += 8.0f * float(nTabSizeInSpaces) * scale.x;
				}
				else
				{
					int32_t ox = (c - 32) % 16;
					int32_t oy = (c - 32) / 16;
					DrawPartialDecal(pos + spos, fontDecal, { float(ox) * 8.0f + float(vFontSpacing[c - 32].x), float(oy) * 8.0f }, { float(vFontSpacing[c - 32].y), 8.0f }, scale, col);
					spos.x += float(vFontSpacing[c - 32].y) * scale.x;
				}
			}
		}

		// Draws a single shaded filled rectangle as a decal
		void FillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel col = olc::WHITE)
		{
			olc::vf2d vNewSize = (size - olc::vf2d(0.375f, 0.375f)).ceil();
			std::array<olc::vf2d, 4> points = { { {pos}, {pos.x, pos.y + vNewSize.y}, {pos + vNewSize}, {pos.x + vNewSize.x, pos.y} } };
			std::array<olc::vf2d, 4> uvs = { {{0,0},{0,0},{0,0},{0,0}} };
			std::array<olc::Pixel, 4> cols = { {col, col, col, col} };
			DrawExplicitDecal(nullptr, points.data(), uvs.data(), cols.data(), 4);
		}

		// Draws a corner shaded rectangle as a decal
		void GradientFillRectDecal(const olc::vf2d& pos, const olc::vf2d& size, const olc::Pixel colTL, const olc::Pixel colBL, const olc::Pixel colBR, const olc::Pixel colTR)
		{
			std::array<olc::vf2d, 4> points = { { {pos}, {pos.x, pos.y + size.y}, {pos + size}, {pos.x + size.x, pos.y} } };
			std::array<olc::vf2d, 4> uvs = { {{0,0},{0,0},{0,0},{0,0}} };
			std::array<olc::Pixel, 4> cols = { {colTL, colBL, colBR, colTR} };
			DrawExplicitDecal(nullptr, points.data(), uvs.data(), cols.data(), 4);
		}

		// Draws an arbitrary convex textured polygon using GPU
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const olc::Pixel tint = olc::WHITE)
		{
			DecalInstance di;
			di.decal = decal;
			di.points = uint32_t(pos.size());
			di.pos.resize(di.points);
			di.uv.resize(di.points);
			di.w.resize(di.points);
			di.tint.resize(di.points);
			for (uint32_t i = 0; i < di.points; i++)
			{
				di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
				di.uv[i] = uv[i];
				di.tint[i] = tint;
				di.w[i] = 1.0f;
			}
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<float>& depth, const std::vector<olc::vf2d>& uv, const olc::Pixel tint = olc::WHITE)
		{
			DecalInstance di;
			di.decal = decal;
			di.points = uint32_t(pos.size());
			di.pos.resize(di.points);
			di.uv.resize(di.points);
			di.w.resize(di.points);
			di.tint.resize(di.points);
			for (uint32_t i = 0; i < di.points; i++)
			{
				di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
				di.uv[i] = uv[i];
				di.tint[i] = tint;
				di.w[i] = 1.0f;
			}
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}
		void DrawPolygonDecal(olc::Decal* decal, const std::vector<olc::vf2d>& pos, const std::vector<olc::vf2d>& uv, const std::vector<olc::Pixel>& tint)
		{
			DecalInstance di;
			di.decal = decal;
			di.points = uint32_t(pos.size());
			di.pos.resize(di.points);
			di.uv.resize(di.points);
			di.w.resize(di.points);
			di.tint.resize(di.points);
			for (uint32_t i = 0; i < di.points; i++)
			{
				di.pos[i] = { (pos[i].x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos[i].y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
				di.uv[i] = uv[i];
				di.tint[i] = tint[i];
				di.w[i] = 1.0f;
			}
			di.mode = nDecalMode;
			di.structure = nDecalStructure;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}

		// Draws a line in Decal Space
		void DrawLineDecal(const olc::vf2d& pos1, const olc::vf2d& pos2, Pixel p = olc::WHITE)
		{
			DecalInstance di;
			di.decal = nullptr;
			di.points = uint32_t(2);
			di.pos.resize(di.points);
			di.uv.resize(di.points);
			di.w.resize(di.points);
			di.tint.resize(di.points);
			di.pos[0] = { (pos1.x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos1.y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[0] = { 0.0f, 0.0f };
			di.tint[0] = p;
			di.w[0] = 1.0f;
			di.pos[1] = { (pos2.x * vInvScreenSize.x) * 2.0f - 1.0f, ((pos2.y * vInvScreenSize.y) * 2.0f - 1.0f) * -1.0f };
			di.uv[1] = { 0.0f, 0.0f };
			di.tint[1] = p;
			di.w[1] = 1.0f;
			di.mode = olc::DecalMode::WIREFRAME;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}

		void DrawRotatedStringDecal(const olc::vf2d& pos, const std::string& sText, const float fAngle, const olc::vf2d& center = { 0.0f, 0.0f }, const olc::Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f })
		{
			olc::vf2d spos = center;
			for (auto c : sText)
			{
				if (c == '\n')
				{
					spos.x = center.x; spos.y -= 8.0f;
				}
				else if (c == '\t')
				{
					spos.x += 8.0f * float(nTabSizeInSpaces) * scale.x;
				}
				else
				{
					int32_t ox = (c - 32) % 16;
					int32_t oy = (c - 32) / 16;
					DrawPartialRotatedDecal(pos, fontDecal, fAngle, spos, { float(ox) * 8.0f, float(oy) * 8.0f }, { 8.0f, 8.0f }, scale, col);
					spos.x -= 8.0f;
				}
			}
		}
		void DrawRotatedStringPropDecal(const olc::vf2d& pos, const std::string& sText, const float fAngle, const olc::vf2d& center = { 0.0f, 0.0f }, const olc::Pixel col = olc::WHITE, const olc::vf2d& scale = { 1.0f, 1.0f })
		{
			olc::vf2d spos = center;
			for (auto c : sText)
			{
				if (c == '\n')
				{
					spos.x = center.x; spos.y -= 8.0f;
				}
				else if (c == '\t')
				{
					spos.x += 8.0f * float(nTabSizeInSpaces) * scale.x;
				}
				else
				{
					int32_t ox = (c - 32) % 16;
					int32_t oy = (c - 32) / 16;
					DrawPartialRotatedDecal(pos, fontDecal, fAngle, spos, { float(ox) * 8.0f + float(vFontSpacing[c - 32].x), float(oy) * 8.0f }, { float(vFontSpacing[c - 32].y), 8.0f }, scale, col);
					spos.x -= float(vFontSpacing[c - 32].y);
				}
			}
		}

		// Clears entire draw target to Pixel
		void Clear(Pixel p)
		{
			for (auto&& a : GetDrawTarget()) a = p;
		}

		// Clears the rendering back buffer
		void ClearBuffer(Pixel p, bool bDepth = true)
		{
			renderer->ClearBuffer(p, bDepth);
		}

		// Returns the font image
		olc::Sprite* GetFontSprite()
		{
			return fontSprite;
		}

		// Clip a line segment to visible area
		bool ClipLineToScreen(olc::vi2d& in_p1, olc::vi2d& in_p2)
		{
			// https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
			static constexpr int SEG_I = 0b0000, SEG_L = 0b0001, SEG_R = 0b0010, SEG_B = 0b0100, SEG_T = 0b1000;
			auto Segment = [&vScreenSize = vScreenSize](const olc::vi2d& v)
			{
				int i = SEG_I;
				if (v.x < 0) i |= SEG_L; else if (v.x > vScreenSize.x) i |= SEG_R;
				if (v.y < 0) i |= SEG_B; else if (v.y > vScreenSize.y) i |= SEG_T;
				return i;
			};


			int s1 = Segment(in_p1), s2 = Segment(in_p2);


			while (true)
			{
				if (!(s1 | s2))	  return true;
				else if (s1 & s2) return false;
				else
				{
					int s3 = s2 > s1 ? s2 : s1;
					olc::vi2d n;
					if (s3 & SEG_T) { n.x = in_p1.x + (in_p2.x - in_p1.x) * (vScreenSize.y - in_p1.y) / (in_p2.y - in_p1.y); n.y = vScreenSize.y; }
					else if (s3 & SEG_B) { n.x = in_p1.x + (in_p2.x - in_p1.x) * (0 - in_p1.y) / (in_p2.y - in_p1.y); n.y = 0; }
					else if (s3 & SEG_R) { n.x = vScreenSize.x; n.y = in_p1.y + (in_p2.y - in_p1.y) * (vScreenSize.x - in_p1.x) / (in_p2.x - in_p1.x); }
					else if (s3 & SEG_L) { n.x = 0; n.y = in_p1.y + (in_p2.y - in_p1.y) * (0 - in_p1.x) / (in_p2.x - in_p1.x); }
					if (s3 == s1) { in_p1 = n; s1 = Segment(in_p1); }
					else { in_p2 = n; s2 = Segment(in_p2); }
				}
			}
			return true;
		}

		// Experimental Lightweight 3D Routines ================
#ifdef OLC_ENABLE_EXPERIMENTAL
		// Set Manual View Matrix
		void LW3D_View(const std::array<float, 16>& m);
		// Set Manual World Matrix
		void LW3D_World(const std::array<float, 16>& m);
		// Set Manual Projection Matrix
		void LW3D_Projection(const std::array<float, 16>& m);

		// Draws a vector of vertices, interprted as individual triangles
		void LW3D_DrawTriangles(olc::Decal* decal, const std::vector<std::array<float, 3>>& pos, const std::vector<olc::vf2d>& tex, const std::vector<olc::Pixel>& col)
		{
			DecalInstance di;
			di.decal = decal;
			di.points = uint32_t(pos.size());
			di.pos.resize(di.points);
			di.uv.resize(di.points);
			di.w.resize(di.points);
			di.tint.resize(di.points);
			for (uint32_t i = 0; i < di.points; i++)
			{
				di.pos[i] = { pos[i][0], pos[i][1] };
				di.w[i] = pos[i][2];
				di.uv[i] = tex[i];
				di.tint[i] = col[i];
			}
			di.mode = DecalMode::MODEL3D;
			vLayers[nTargetLayer].vecDecalInstance.push_back(di);
		}

		void LW3D_ModelTranslate(const float x, const float y, const float z);

		// Camera convenience functions
		void LW3D_SetCameraAtTarget(const float fEyeX, const float fEyeY, const float fEyeZ,
			const float fTargetX, const float fTargetY, const float fTargetZ,
			const float fUpX = 0.0f, const float fUpY = 1.0f, const float fUpZ = 0.0f);
		void LW3D_SetCameraAlongDirection(const float fEyeX, const float fEyeY, const float fEyeZ,
			const float fDirX, const float fDirY, const float fDirZ,
			const float fUpX = 0.0f, const float fUpY = 1.0f, const float fUpZ = 0.0f);

		// 3D Rendering Flags
		void LW3D_EnableDepthTest(const bool bEnableDepth);
		void LW3D_EnableBackfaceCulling(const bool bEnableCull);
#endif
	public: // Branding
		std::string sAppName;

	private: // Inner mysterious workings
		olc::Sprite* pDrawTarget = nullptr;
		Pixel::Mode	nPixelMode = Pixel::Mode::NORMAL;
		float		fBlendFactor = 1.0f;
		olc::vi2d	vScreenSize = { 256, 240 };
		olc::vf2d	vInvScreenSize = { 1.0f / 256.0f, 1.0f / 240.0f };
		olc::vi2d	vPixelSize = { 4, 4 };
		olc::vi2d   vScreenPixelSize = { 4, 4 };
		olc::vi2d	vMousePos = { 0, 0 };
		int32_t		nMouseWheelDelta = 0;
		olc::vi2d	vMousePosCache = { 0, 0 };
		olc::vi2d   vMouseWindowPos = { 0, 0 };
		int32_t		nMouseWheelDeltaCache = 0;
		olc::vi2d	vWindowSize = { 0, 0 };
		olc::vi2d	vViewPos = { 0, 0 };
		olc::vi2d	vViewSize = { 0,0 };
		bool		bFullScreen = false;
		olc::vf2d	vPixel = { 1.0f, 1.0f };
		bool		bHasInputFocus = false;
		bool		bHasMouseFocus = false;
		bool		bEnableVSYNC = false;
		float		fFrameTimer = 1.0f;
		float		fLastElapsed = 0.0f;
		int			nFrameCount = 0;
		Sprite* fontSprite = nullptr;
		Decal* fontDecal = nullptr;
		std::vector<LayerDesc> vLayers;
		uint8_t		nTargetLayer = 0;
		uint32_t	nLastFPS = 0;
		bool        bPixelCohesion = false;
		DecalMode   nDecalMode = DecalMode::NORMAL;
		DecalStructure nDecalStructure = DecalStructure::FAN;
		std::function<olc::Pixel(const int x, const int y, const olc::Pixel&, const olc::Pixel&)> funcPixelMode;
		std::chrono::time_point<std::chrono::system_clock> m_tp1, m_tp2;
		std::vector<olc::vi2d> vFontSpacing;

		// State of keyboard		
		bool		pKeyNewState[256] = { 0 };
		bool		pKeyOldState[256] = { 0 };
		HWButton	pKeyboardState[256] = { 0 };

		// State of mouse
		bool		pMouseNewState[nMouseButtons] = { 0 };
		bool		pMouseOldState[nMouseButtons] = { 0 };
		HWButton	pMouseState[nMouseButtons] = { 0 };

		// The main engine thread
		void		EngineThread()
		{
			// Allow platform to do stuff here if needed, since its now in the
			// context of this thread
			if (platform->ThreadStartUp() == olc::FAIL)	return;


			// Do engine context specific initialisation
			olc_PrepareEngine();


			// Create user resources as part of this thread
			for (auto& ext : vExtensions) ext->OnBeforeUserCreate();
			if (!OnUserCreate()) bAtomActive = false;
			for (auto& ext : vExtensions) ext->OnAfterUserCreate();


			while (bAtomActive)
			{
				// Run as fast as possible
				while (bAtomActive) { olc_CoreUpdate(); }


				// Allow the user to free resources if they have overrided the destroy function
				if (!OnUserDestroy())
				{
					// User denied destroy for some reason, so continue running
					bAtomActive = true;
				}
			}


			platform->ThreadCleanUp();
		}


		// If anything sets this flag to false, the engine
		// "should" shut down gracefully
		static std::atomic<bool> bAtomActive;

	public:
		// "Break In" Functions
		void olc_UpdateMouse(int32_t x, int32_t y)
		{
			// Mouse coords come in screen space
			// But leave in pixel space
			bHasMouseFocus = true;
			vMouseWindowPos = { x, y };
			// Full Screen mode may have a weird viewport we must clamp to
			x -= vViewPos.x;
			y -= vViewPos.y;
			vMousePosCache.x = (int32_t)(((float)x / (float)(vWindowSize.x - (vViewPos.x * 2)) * (float)vScreenSize.x));
			vMousePosCache.y = (int32_t)(((float)y / (float)(vWindowSize.y - (vViewPos.y * 2)) * (float)vScreenSize.y));
			if (vMousePosCache.x >= (int32_t)vScreenSize.x)	vMousePosCache.x = vScreenSize.x - 1;
			if (vMousePosCache.y >= (int32_t)vScreenSize.y)	vMousePosCache.y = vScreenSize.y - 1;
			if (vMousePosCache.x < 0) vMousePosCache.x = 0;
			if (vMousePosCache.y < 0) vMousePosCache.y = 0;
		}
		void olc_UpdateMouseWheel(int32_t delta)
		{
			nMouseWheelDeltaCache += delta;
		}
		void olc_UpdateWindowSize(int32_t x, int32_t y)
		{
			vWindowSize = { x, y };
			olc_UpdateViewport();
		}
		void olc_UpdateViewport()
		{
			int32_t ww = vScreenSize.x * vPixelSize.x;
			int32_t wh = vScreenSize.y * vPixelSize.y;
			float wasp = (float)ww / (float)wh;


			if (bPixelCohesion)
			{
				vScreenPixelSize = (vWindowSize / vScreenSize);
				vViewSize = (vWindowSize / vScreenSize) * vScreenSize;
			}
			else
			{
				vViewSize.x = (int32_t)vWindowSize.x;
				vViewSize.y = (int32_t)((float)vViewSize.x / wasp);


				if (vViewSize.y > vWindowSize.y)
				{
					vViewSize.y = vWindowSize.y;
					vViewSize.x = (int32_t)((float)vViewSize.y * wasp);
				}
			}


			vViewPos = (vWindowSize - vViewSize) / 2;
		}
		void olc_ConstructFontSheet()
		{
			std::string data;
			data += "?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000";
			data += "O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400";
			data += "Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000";
			data += "720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000";
			data += "OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000";
			data += "ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000";
			data += "Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000";
			data += "70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000";
			data += "OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000";
			data += "00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000";
			data += "<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000";
			data += "O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000";
			data += "00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000";
			data += "Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0";
			data += "O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000";
			data += "?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";


			fontSprite = new olc::Sprite(128, 48);
			int px = 0, py = 0;
			for (size_t b = 0; b < 1024; b += 4)
			{
				uint32_t sym1 = (uint32_t)data[b + 0] - 48;
				uint32_t sym2 = (uint32_t)data[b + 1] - 48;
				uint32_t sym3 = (uint32_t)data[b + 2] - 48;
				uint32_t sym4 = (uint32_t)data[b + 3] - 48;
				uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;


				for (int i = 0; i < 24; i++)
				{
					int k = r & (1 << i) ? 255 : 0;
					fontSprite->SetPixel(px, py, olc::Pixel(k, k, k, k));
					if (++py == 48) { px++; py = 0; }
				}
			}


			fontDecal = new olc::Decal(fontSprite);


			constexpr std::array<uint8_t, 96> vSpacing = { {
				0x03,0x25,0x16,0x08,0x07,0x08,0x08,0x04,0x15,0x15,0x08,0x07,0x15,0x07,0x24,0x08,
				0x08,0x17,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x24,0x15,0x06,0x07,0x16,0x17,
				0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x17,0x08,0x08,0x17,0x08,0x08,0x08,
				0x08,0x08,0x08,0x08,0x17,0x08,0x08,0x08,0x08,0x17,0x08,0x15,0x08,0x15,0x08,0x08,
				0x24,0x18,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x33,0x17,0x17,0x33,0x18,0x17,0x17,
				0x17,0x17,0x17,0x17,0x07,0x17,0x17,0x18,0x18,0x17,0x17,0x07,0x33,0x07,0x08,0x00, } };


			for (auto c : vSpacing) vFontSpacing.push_back({ c >> 4, c & 15 });


		}
		void olc_CoreUpdate()
		{
			// Handle Timing
			m_tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = m_tp2 - m_tp1;
			m_tp1 = m_tp2;


			// Our time per frame coefficient
			float fElapsedTime = elapsedTime.count();
			fLastElapsed = fElapsedTime;


			// Some platforms will need to check for events
			platform->HandleSystemEvent();


			// Compare hardware input states from previous frame
			auto ScanHardware = [&](HWButton* pKeys, bool* pStateOld, bool* pStateNew, uint32_t nKeyCount)
			{
				for (uint32_t i = 0; i < nKeyCount; i++)
				{
					pKeys[i].bPressed = false;
					pKeys[i].bReleased = false;
					if (pStateNew[i] != pStateOld[i])
					{
						if (pStateNew[i])
						{
							pKeys[i].bPressed = !pKeys[i].bHeld;
							pKeys[i].bHeld = true;
						}
						else
						{
							pKeys[i].bReleased = true;
							pKeys[i].bHeld = false;
						}
					}
					pStateOld[i] = pStateNew[i];
				}
			};


			ScanHardware(pKeyboardState, pKeyOldState, pKeyNewState, 256);
			ScanHardware(pMouseState, pMouseOldState, pMouseNewState, nMouseButtons);


			// Cache mouse coordinates so they remain consistent during frame
			vMousePos = vMousePosCache;
			nMouseWheelDelta = nMouseWheelDeltaCache;
			nMouseWheelDeltaCache = 0;


			//	renderer->ClearBuffer(olc::BLACK, true);


			// Handle Frame Update
			bool bExtensionBlockFrame = false;
			for (auto& ext : vExtensions) bExtensionBlockFrame |= ext->OnBeforeUserUpdate(fElapsedTime);
			if (!bExtensionBlockFrame)
			{
				if (!OnUserUpdate(fElapsedTime)) bAtomActive = false;
			}
			for (auto& ext : vExtensions) ext->OnAfterUserUpdate(fElapsedTime);


			// Display Frame
			renderer->UpdateViewport(vViewPos, vViewSize);
			renderer->ClearBuffer(olc::BLACK, true);


			// Layer 0 must always exist
			vLayers[0].bUpdate = true;
			vLayers[0].bShow = true;
			SetDecalMode(DecalMode::NORMAL);
			renderer->PrepareDrawing();


			for (auto layer = vLayers.rbegin(); layer != vLayers.rend(); ++layer)
			{
				if (layer->bShow)
				{
					if (layer->funcHook == nullptr)
					{
						renderer->ApplyTexture(layer->pDrawTarget.Decal()->id);
						if (layer->bUpdate)
						{
							layer->pDrawTarget.Decal()->Update();
							layer->bUpdate = false;
						}


						renderer->DrawLayerQuad(layer->vOffset, layer->vScale, layer->tint);


						// Display Decals in order for this layer
						for (auto& decal : layer->vecDecalInstance)
							renderer->DrawDecal(decal);
						layer->vecDecalInstance.clear();
					}
					else
					{
						// Mwa ha ha.... Have Fun!!!
						layer->funcHook();
					}
				}
			}


			// Present Graphics to screen
			renderer->DisplayFrame();


			// Update Title Bar
			fFrameTimer += fElapsedTime;
			nFrameCount++;
			if (fFrameTimer >= 1.0f)
			{
				nLastFPS = nFrameCount;
				fFrameTimer -= 1.0f;
				std::string sTitle = "OneLoneCoder.com - Pixel Game Engine - " + sAppName + " - FPS: " + std::to_string(nFrameCount);
				platform->SetWindowTitle(sTitle);
				nFrameCount = 0;
			}
		}
		void olc_PrepareEngine()
		{
			// Start OpenGL, the context is owned by the game thread
			if (platform->CreateGraphics(bFullScreen, bEnableVSYNC, vViewPos, vViewSize) == olc::FAIL) return;


			// Construct default font sheet
			olc_ConstructFontSheet();


			// Create Primary Layer "0"
			CreateLayer();
			vLayers[0].bUpdate = true;
			vLayers[0].bShow = true;
			SetDrawTarget(nullptr);


			m_tp1 = std::chrono::system_clock::now();
			m_tp2 = std::chrono::system_clock::now();
		}
		void olc_UpdateMouseState(int32_t button, bool state)
		{
			pMouseNewState[button] = state;
		}
		void olc_UpdateKeyState(int32_t key, bool state)
		{
			pKeyNewState[key] = state;
		}
		void olc_UpdateMouseFocus(bool state)
		{
			bHasMouseFocus = state;
		}
		void olc_UpdateKeyFocus(bool state)
		{
			bHasInputFocus = state;
		}
		void olc_Terminate()
		{
			bAtomActive = false;
		}
		void olc_Reanimate()
		{
			bAtomActive = true;
		}
		bool olc_IsRunning()
		{
			return bAtomActive;
		}

		// At the very end of this file, chooses which
		// components to compile
		virtual void olc_ConfigureSystem()
		{
#if !defined(OLC_PGE_HEADLESS)

#if defined(OLC_IMAGE_GDI)
			olc::Sprite::loader = std::make_unique<olc::ImageLoader_GDIPlus>();
#endif

#if defined(OLC_IMAGE_LIBPNG)
			olc::Sprite::loader = std::make_unique<olc::ImageLoader_LibPNG>();
#endif

#if defined(OLC_IMAGE_STB)
			olc::Sprite::loader = std::make_unique<olc::ImageLoader_STB>();
#endif

#if defined(OLC_IMAGE_CUSTOM_EX)
			olc::Sprite::loader = std::make_unique<OLC_IMAGE_CUSTOM_EX>();
#endif




#if defined(OLC_PLATFORM_WINAPI)
			olc::platform = std::make_unique<olc::Platform_Windows>();
#endif

#if defined(OLC_PLATFORM_X11)
			olc::platform = std::make_unique<olc::Platform_Linux>();
#endif

#if defined(OLC_PLATFORM_GLUT)
			olc::platform = std::make_unique<olc::Platform_GLUT>();
#endif

#if defined(OLC_PLATFORM_EMSCRIPTEN)
			olc::platform = std::make_unique<olc::Platform_Emscripten>();
#endif

#if defined(OLC_PLATFORM_CUSTOM_EX)
			olc::platform = std::make_unique<OLC_PLATFORM_CUSTOM_EX>();
#endif



#if defined(OLC_GFX_OPENGL10)
			olc::renderer = std::make_unique<olc::Renderer_OGL10>();
#endif

#if defined(OLC_GFX_OPENGL33)
			olc::renderer = std::make_unique<olc::Renderer_OGL33>();
#endif

#if defined(OLC_GFX_OPENGLES2)
			olc::renderer = std::make_unique<olc::Renderer_OGLES2>();
#endif

#if defined(OLC_GFX_DIRECTX10)
			olc::renderer = std::make_unique<olc::Renderer_DX10>();
#endif

#if defined(OLC_GFX_DIRECTX11)
			olc::renderer = std::make_unique<olc::Renderer_DX11>();
#endif

#if defined(OLC_GFX_CUSTOM_EX)
			olc::renderer = std::make_unique<OLC_RENDERER_CUSTOM_EX>();
#endif

			// Associate components with PGE instance
			olc::platform->ptrPGE = this;
			olc::renderer->ptrPGE = this;
#else
			olc::Sprite::loader = nullptr;
			olc::platform = nullptr;
			olc::renderer = nullptr;
#endif
		}

		// NOTE: Items Here are to be deprecated, I have left them in for now
		// in case you are using them, but they will be removed.
		// olc::vf2d	vSubPixelOffset = { 0.0f, 0.0f };

	public: // PGEX Stuff
		friend class PGEX;
		void pgex_Register(olc::PGEX* pgex)
		{
			if (std::find(vExtensions.begin(), vExtensions.end(), pgex) == vExtensions.end())
				vExtensions.push_back(pgex);
		}

	private:
		std::vector<olc::PGEX*> vExtensions;
	};

	std::atomic<bool> PixelGameEngine::bAtomActive{ false };
}

#pragma endregion

#if !defined(PGE_USE_CUSTOM_START)
olc::rcode olc::PixelGameEngine::Start()
{
	if (platform->ApplicationStartUp() != olc::OK) return olc::FAIL;


	// Construct the window
	if (platform->CreateWindowPane({ 30,30 }, vWindowSize, bFullScreen) != olc::OK) return olc::FAIL;
	olc_UpdateWindowSize(vWindowSize.x, vWindowSize.y);


	// Start the thread
	bAtomActive = true;
	std::thread t = std::thread(&PixelGameEngine::EngineThread, this);


	// Some implementations may form an event loop here
	platform->StartSystemEventLoop();


	// Wait for thread to be exited
	t.join();


	if (platform->ApplicationCleanUp() != olc::OK) return olc::FAIL;


	return olc::OK;
}
#endif