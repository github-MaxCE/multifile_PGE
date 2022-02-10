#pragma once
#ifndef OLCDECAL_H
#define OLCDECAL_H
#include <stdint.h>
#include <vector>
#include "olcV2D.h"
#include "olcPixel.h"
#include "olcSprite.h"

namespace olc
{
	// O------------------------------------------------------------------------------O
	// | olc::Decal - A GPU resident storage of an olc::Sprite                        |
	// O------------------------------------------------------------------------------O
	class Decal
	{
	public:
		Decal(olc::Sprite* spr, bool filter = false, bool clamp = true);
		Decal(const uint32_t nExistingTextureResource, olc::Sprite* spr);
		virtual ~Decal();
		void Update();
		void UpdateSprite();

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
}
#endif //OLCDECAL_H