#pragma once
#include <stdint.h>
#include <vector>
#include "olcRenderable.h"
#include "olcDecal.h"

namespace olc
{
	class Pixel;
	// Pixel Game Engine Advanced Configuration
	constexpr uint8_t  nMouseButtons = 5;
	constexpr uint8_t  nDefaultAlpha = 0xFF;
	constexpr uint32_t nDefaultPixel = (nDefaultAlpha << 24);
	constexpr uint8_t  nTabSizeInSpaces = 4;
	enum rcode { FAIL = 0, OK = 1, NO_FILE = -1 };

	// O------------------------------------------------------------------------------O
	// | Auxilliary components internal to engine                                     |
	// O------------------------------------------------------------------------------O

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
}