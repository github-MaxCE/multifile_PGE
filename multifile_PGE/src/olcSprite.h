#pragma once
#include <memory>
#include <stdint.h>
#include "olcPixel.h"
#include "olcImageLoader.h"
namespace olc
{
	// O------------------------------------------------------------------------------O
	// | olc::Sprite - An image represented by a 2D array of olc::Pixel               |
	// O------------------------------------------------------------------------------O
	class Sprite
	{
	public:
		Sprite();
		Sprite(const std::string& sImageFile, olc::ResourcePack* pack = nullptr);
		Sprite(int32_t w, int32_t h);
		Sprite(const olc::Sprite&) = delete;
		~Sprite();

	public:
		olc::rcode LoadFromFile(const std::string& sImageFile, olc::ResourcePack* pack = nullptr);

	public:
		int32_t width = 0;
		int32_t height = 0;
		enum Mode { NORMAL, PERIODIC, CLAMP };
		enum Flip { NONE = 0, HORIZ = 1, VERT = 2 };

	public:
		void SetSampleMode(olc::Sprite::Mode mode = olc::Sprite::Mode::NORMAL);
		olc::Pixel GetPixel(int32_t x, int32_t y) const;
		bool  SetPixel(int32_t x, int32_t y, olc::Pixel p);
		olc::Pixel GetPixel(const olc::vi2d& a) const;
		bool  SetPixel(const olc::vi2d& a, olc::Pixel p);
		olc::Pixel Sample(float x, float y) const;
		olc::Pixel SampleBL(float u, float v) const;
		olc::Pixel* GetData();
		olc::Sprite* Duplicate();
		olc::Sprite* Duplicate(const olc::vi2d& vPos, const olc::vi2d& vSize);
		std::vector<olc::Pixel> pColData;
		Mode modeSample = Mode::NORMAL;

		static std::unique_ptr<olc::ImageLoader> loader;
	};
}