#include "olcPixelGameEngine.h"

namespace olc
{
	// O------------------------------------------------------------------------------O
	// | olc::Decal IMPLEMENTATION                                                    |
	// O------------------------------------------------------------------------------O
	Decal::Decal(olc::Sprite* spr, bool filter, bool clamp)
	{
		id = -1;
		if (spr == nullptr) return;
		sprite = spr;
		id = olc::renderer->CreateTexture(sprite->width, sprite->height, filter, clamp);
		Update();
	}

	Decal::Decal(const uint32_t nExistingTextureResource, olc::Sprite* spr)
	{
		if (spr == nullptr) return;
		id = nExistingTextureResource;
	}

	void Decal::Update()
	{
		if (sprite == nullptr) return;
		vUVScale = { 1.0f / float(sprite->width), 1.0f / float(sprite->height) };
		olc::renderer->ApplyTexture(id);
		olc::renderer->UpdateTexture(id, sprite);
	}

	void Decal::UpdateSprite()
	{
		if (sprite == nullptr) return;
		olc::renderer->ApplyTexture(id);
		olc::renderer->ReadTexture(id, sprite);
	}

	Decal::~Decal()
	{
		if (id != -1)
		{
			olc::renderer->DeleteTexture(id);
			id = -1;
		}
	}
}