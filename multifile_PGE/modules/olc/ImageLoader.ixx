export module olc.ImageLoader;

import std.core;
import olc.xtra;
import olc.ResourcePack;
import olc.Pixel;

export namespace olc
{
	class ImageLoader
	{
	public:
		ImageLoader() = default;
		virtual ~ImageLoader() = default;
		virtual std::vector<std::vector<olc::Pixel>>& LoadImageResource(const std::string& sImageFile, olc::ResourcePack* pack = nullptr) = 0;
	};
}