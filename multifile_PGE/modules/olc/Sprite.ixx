export module olc.Sprite;

import std.core;
import std.memory;
import olc.xtra;
import olc.V2D;
import olc.Pixel;
import olc.ResourcePack;
import olc.ImageLoader;

export namespace olc
{
	// O------------------------------------------------------------------------------O
	// | olc::Sprite - An image represented by a 2D array of olc::Pixel               |
	// O------------------------------------------------------------------------------O
	class Sprite
	{
		using iterator = std::vector<olc::Pixel>::iterator;
	public:
		Sprite()
		{
			width = 0; height = 0;
		}

		Sprite(const std::string& sImageFile, olc::ResourcePack* pack = nullptr)
		{
			LoadFromFile(sImageFile, pack);
		}

		Sprite(olc::vu2d size) :
			Sprite(size.x, size.y)
		{}


		Sprite(uint32_t w, uint32_t h) :
			width(w),
			height(h)
		{
			pColData.clear();
			pColData.resize(width * height, nDefaultPixel);
		}

		Sprite(const olc::Sprite&) = delete;

		~Sprite()
		{
			pColData.clear();
		}

	public:
		olc::rcode LoadFromFile(const std::string& sImageFile, olc::ResourcePack* pack = nullptr)
		{
			auto vec = loader->LoadImageResource(sImageFile, pack);
			if (vec.empty()) return olc::rcode::FAIL;
			height = vec.size();
			width  = vec[0].size();
			pColData.clear();
			for (auto& a : vec)
				for (auto& b : a)
					pColData.push_back(b);
			return olc::rcode::OK;
		}

	public:
		union
		{
			olc::vu2d size = olc::vu2d(0, 0);
			struct
			{
				uint32_t width;
				uint32_t height;
			};
		};
		enum class Mode { NORMAL, PERIODIC, CLAMP };
		enum Flip : uint8_t { NONE, HORIZ, VERT };

	public:
		iterator begin()
		{
			return pColData.begin();
		}

		iterator end()
		{
			return pColData.end();
		}

		void SetSampleMode(olc::Sprite::Mode mode = olc::Sprite::Mode::NORMAL)
		{
			modeSample = mode;
		}

		olc::Pixel GetPixel(uint32_t x, uint32_t y) const
		{
			if (modeSample == olc::Sprite::Mode::NORMAL)
			{
				if (x >= 0 && x < width && y >= 0 && y < height)
					return pColData[y * width + x];
				else
					return Pixel(0, 0, 0, 0);
			}
			else
			{
				if (modeSample == olc::Sprite::Mode::PERIODIC)
					return pColData[uint32_t(abs(int(y % height))) * width + uint32_t(abs(int(x % width)))];
				else
					return pColData[std::max<uint32_t>(0, std::min<uint32_t>(y, height - 1)) * width + std::max<uint32_t>(0, std::min<uint32_t>(x, width - 1))];
			}
		}

		bool  SetPixel(uint32_t x, uint32_t y, olc::Pixel p)
		{
			if (x >= 0 && x < width && y >= 0 && y < height)
			{
				pColData[y * width + x] = p;
				return true;
			}
			else
				return false;
		}

		olc::Pixel GetPixel(const olc::vu2d& a) const
		{
			return GetPixel(a.x, a.y);
		}

		bool  SetPixel(const olc::vu2d& a, olc::Pixel p)
		{
			return SetPixel(a.x, a.y, p);
		}

		olc::Pixel Sample(float x, float y) const
		{
			uint32_t sx = std::min<uint32_t>((uint32_t)((x * (float)width)), width - 1);
			uint32_t sy = std::min<uint32_t>((uint32_t)((y * (float)height)), height - 1);
			return GetPixel(sx, sy);
		}

		olc::Pixel SampleBL(float u, float v) const
		{
			u = u * width - 0.5f;
			v = v * height - 0.5f;
			int x = (int)floor(u); // cast to int rounds toward zero, not downward
			int y = (int)floor(v); // Thanks @joshinils
			float u_ratio = u - x;
			float v_ratio = v - y;
			float u_opposite = 1 - u_ratio;
			float v_opposite = 1 - v_ratio;

			olc::Pixel p1 = GetPixel(std::max<uint32_t>(x, 0), std::max<uint32_t>(y, 0));
			olc::Pixel p2 = GetPixel(std::min<uint32_t>(x + 1, (int)width - 1), std::max<uint32_t>(y, 0));
			olc::Pixel p3 = GetPixel(std::max<uint32_t>(x, 0), std::min<uint32_t>(y + 1, (int)height - 1));
			olc::Pixel p4 = GetPixel(std::min<uint32_t>(x + 1, (int)width - 1), std::min<uint32_t>(y + 1, (int)height - 1));

			return olc::Pixel(
				(uint8_t)((p1.r * u_opposite + p2.r * u_ratio) * v_opposite + (p3.r * u_opposite + p4.r * u_ratio) * v_ratio),
				(uint8_t)((p1.g * u_opposite + p2.g * u_ratio) * v_opposite + (p3.g * u_opposite + p4.g * u_ratio) * v_ratio),
				(uint8_t)((p1.b * u_opposite + p2.b * u_ratio) * v_opposite + (p3.b * u_opposite + p4.b * u_ratio) * v_ratio));
		}

		olc::Pixel* GetData()
		{
			return pColData.data();
		}

		olc::Sprite* Duplicate()
		{
			olc::Sprite* spr = new olc::Sprite(width, height);
			std::memcpy(spr->GetData(), GetData(), width * height * sizeof(olc::Pixel));
			spr->modeSample = modeSample;
			return spr;
		}

		olc::Sprite* Duplicate(const olc::vu2d& vPos, const olc::vu2d& vSize)
		{
			olc::Sprite* spr = new olc::Sprite(vSize.x, vSize.y);
			for (int y = 0; y < vSize.y; y++)
				for (int x = 0; x < vSize.x; x++)
					spr->SetPixel(x, y, GetPixel(vPos.x + x, vPos.y + y));
			return spr;
		}

		std::vector<olc::Pixel> pColData;
		olc::Sprite::Mode modeSample = olc::Sprite::Mode::NORMAL;

		static std::unique_ptr<olc::ImageLoader> loader;
	};

	std::unique_ptr<olc::ImageLoader> olc::Sprite::loader = nullptr;
}