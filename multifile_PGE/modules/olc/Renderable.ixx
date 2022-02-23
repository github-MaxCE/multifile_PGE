export module olc.Renderable;

import std.core;
import olc.xtra;
import olc.ResourcePack;
import olc.Sprite;

export namespace olc
{
	class Decal;

	// O------------------------------------------------------------------------------O
	// | olc::Renderable - Convenience class to keep a sprite and decal together      |
	// O------------------------------------------------------------------------------O
	class Renderable
	{
	public:
		Renderable() = default;
		Renderable(Renderable&& r) : pSprite(std::move(r.pSprite)), pDecal(std::move(r.pDecal)) {}
		Renderable(const Renderable&) = delete;

		olc::rcode Load(const std::string& sFile, ResourcePack* pack = nullptr, bool filter = false, bool clamp = true)
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

		void Create(uint32_t width, uint32_t height, bool filter = false, bool clamp = true)
		{
			pSprite = std::make_unique<olc::Sprite>(width, height);
			pDecal = std::make_unique<olc::Decal>(pSprite.get(), filter, clamp);
		}

		olc::Decal* Decal() const
		{
			return pDecal.get();
		}
		olc::Sprite* Sprite() const
		{
			return pSprite.get();
		}

	private:
		std::unique_ptr<olc::Sprite> pSprite = nullptr;
		std::unique_ptr<olc::Decal> pDecal = nullptr;
	};
}