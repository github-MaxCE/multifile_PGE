export module olc.ImageLoader;

import std.core;
import olc.xtra;
import olc.ResourcePack;
import olc.Sprite;

export namespace olc
{
	class ImageLoader
	{
	public:
		ImageLoader() = default;
		virtual ~ImageLoader() = default;
		virtual olc::rcode LoadImageResource(olc::Sprite* spr, const std::string& sImageFile, olc::ResourcePack* pack) = 0;
		virtual olc::rcode SaveImageResource(olc::Sprite* spr, const std::string& sImageFile) = 0;
	};
}