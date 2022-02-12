#include "olcPixelGameEngine.h"

namespace olc
{
	void Renderable::Create(uint32_t width, uint32_t height, bool filter, bool clamp)
	{
		pSprite = std::make_unique<olc::Sprite>(width, height);
		pDecal = std::make_unique<olc::Decal>(pSprite.get(), filter, clamp);
	}

	olc::rcode Renderable::Load(const std::string& sFile, ResourcePack* pack, bool filter, bool clamp)
	{
		pSprite = std::make_unique<olc::Sprite>();
		if (pSprite->LoadFromFile(sFile, pack) == olc::rcode::OK)
		{
			pDecal = std::make_unique<olc::Decal>(pSprite.get(), filter, clamp);
			return olc::rcode::OK;
		}
		else
		{
			pSprite.release();
			pSprite = nullptr;
			return olc::rcode::NO_FILE;
		}
	}

	olc::Decal* Renderable::Decal() const
	{
		return pDecal.get();
	}

	olc::Sprite* Renderable::Sprite() const
	{
		return pSprite.get();
	}
}