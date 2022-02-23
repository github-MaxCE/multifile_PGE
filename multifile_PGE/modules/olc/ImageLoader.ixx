module;

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


// O------------------------------------------------------------------------------O
// | olcPixelGameEngine Image loaders                                             |
// O------------------------------------------------------------------------------O
#pragma region imageloaders
#pragma region gdi
// O------------------------------------------------------------------------------O
// | START IMAGE LOADER: GDI+, Windows Only, always exists, a little slow         |
// O------------------------------------------------------------------------------O
#if defined(OLC_IMAGE_GDI)

#define min(a, b) ((a < b) ? a : b)
#define max(a, b) ((a > b) ? a : b)
#include <objidl.h>
#include <gdiplus.h>
#if defined(__MINGW32__) // Thanks Gusgo & Dandistine, but c'mon mingw!! wtf?!
#include <gdiplus/gdiplusinit.h>
#else
#include <gdiplusinit.h>
#endif
#include <shlwapi.h>
#undef min
#undef max

#if !defined(__MINGW32__)
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "Shlwapi.lib")
#endif


namespace olc
{
	// Thanks @MaGetzUb for this, which allows sprites to be defined
	// at construction, by initialising the GDI subsystem
	static class GDIPlusStartup
	{
	public:
		GDIPlusStartup()
		{
			Gdiplus::GdiplusStartupInput startupInput;
			GdiplusStartup(&token, &startupInput, NULL);
		}


		ULONG_PTR	token;

		~GDIPlusStartup()
		{
			// Well, MarcusTU thought this was important :D
			Gdiplus::GdiplusShutdown(token);
		}
	} gdistartup;


	export class ImageLoader_GDIPlus : public olc::ImageLoader
	{
	private:
		std::wstring ConvertS2W(std::string s)
		{
#ifdef __MINGW32__
			wchar_t* buffer = new wchar_t[s.length() + 1];
			mbstowcs(buffer, s.c_str(), s.length());
			buffer[s.length()] = L'\0';
#else
			int count = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
			wchar_t* buffer = new wchar_t[count];
			MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, count);
#endif
			std::wstring w(buffer);
			delete[] buffer;
			return w;
		}


	public:
		ImageLoader_GDIPlus() : ImageLoader()
		{}


		std::vector<std::vector<olc::Pixel>>& LoadImageResource(const std::string& sImageFile, olc::ResourcePack* pack) override
		{
			// Open file
			Gdiplus::Bitmap* bmp = nullptr;
			if (pack != nullptr)
			{
				// Load sprite from input stream
				ResourceBuffer rb = pack->GetFileBuffer(sImageFile);
				bmp = Gdiplus::Bitmap::FromStream(SHCreateMemStream((BYTE*)rb.vMemory.data(), UINT(rb.vMemory.size())));
			}
			else
			{
				// Check file exists
				if (!_gfs::exists(sImageFile)) return olc::rcode::NO_FILE;


				// Load sprite from file
				bmp = Gdiplus::Bitmap::FromFile(ConvertS2W(sImageFile).c_str());
			}


			if (bmp->GetLastStatus() != Gdiplus::Ok) return olc::rcode::FAIL;

			std::vector<std::vector<olc::Pixel>> spr(bmp->GetHeight(), std::vector<olc::Pixel>(bmp->GetWidth()));

			for (int y = 0; y < bmp->GetHeight(); y++)
			{
				for (int x = 0; x < bmp->GetWidth(); x++)
				{
					Gdiplus::Color c;
					bmp->GetPixel(x, y, &c);
					spr[y][x] = olc::Pixel(c.GetRed(), c.GetGreen(), c.GetBlue(), c.GetAlpha());
				}
			}
			delete bmp;
			return spr;
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END IMAGE LOADER: GDI+                                                       |
// O------------------------------------------------------------------------------O
#pragma endregion
#pragma region libpng
// O------------------------------------------------------------------------------O
// | START IMAGE LOADER: libpng, default on linux, requires -lpng  (libpng-dev)   |
// O------------------------------------------------------------------------------O
#if defined(OLC_IMAGE_LIBPNG)
#include <png.h>
namespace olc
{
	void pngReadStream(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		png_voidp a = png_get_io_ptr(pngPtr);
		((std::istream*)a)->read((char*)data, length);
	}


	export class ImageLoader_LibPNG : public olc::ImageLoader
	{
	public:
		ImageLoader_LibPNG() : ImageLoader()
		{}

		std::vector<std::vector<olc::Pixel>>& LoadImageResource(const std::string& sImageFile, olc::ResourcePack* pack) override
		{
			////////////////////////////////////////////////////////////////////////////
			// Use libpng, Thanks to Guillaume Cottenceau
			// https://gist.github.com/niw/5963798
			// Also reading png from streams
			// http://www.piko3d.net/tutorials/libpng-tutorial-loading-png-files-from-streams/
			png_structp png;
			png_infop info;

			std::vector<std::vector<olc::Pixel>> spr;

			uint32_t width, height;

			auto loadPNG = [&]()
			{
				png_read_info(png, info);
				png_byte color_type;
				png_byte bit_depth;
				png_bytep* row_pointers;
				width = png_get_image_width(png, info);
				height = png_get_image_height(png, info);
				color_type = png_get_color_type(png, info);
				bit_depth = png_get_bit_depth(png, info);
				if (bit_depth == 16) png_set_strip_16(png);
				if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
				if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)	png_set_expand_gray_1_2_4_to_8(png);
				if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
				if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
					png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
				if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
					png_set_gray_to_rgb(png);
				png_read_update_info(png, info);
				row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
				for (int y = 0; y < height; y++) {
					row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
				}
				png_read_image(png, row_pointers);
				////////////////////////////////////////////////////////////////////////////
				// Create sprite array

				spr.resize(height, std::vector<olc::Pixel>(width));

				// Iterate through image rows, converting into sprite format
				for (int y = 0; y < height; y++)
				{
					png_bytep row = row_pointers[y];
					for (int x = 0; x < width; x++)
					{
						png_bytep px = &(row[x * 4]);
						spr[y][x] = olc::Pixel(px[0], px[1], px[2], px[3]);
					}
				}


				for (int y = 0; y < height; y++) // Thanks maksym33
					free(row_pointers[y]);
				free(row_pointers);
				png_destroy_read_struct(&png, &info, nullptr);
			};


			png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png) goto fail_load;


			info = png_create_info_struct(png);
			if (!info) goto fail_load;


			if (setjmp(png_jmpbuf(png))) goto fail_load;


			if (pack == nullptr)
			{
				FILE* f = fopen(sImageFile.c_str(), "rb");
				if (!f) return olc::rcode::NO_FILE;
				png_init_io(png, f);
				loadPNG();
				fclose(f);
			}
			else
			{
				ResourceBuffer rb = pack->GetFileBuffer(sImageFile);
				std::istream is(&rb);
				png_set_read_fn(png, (png_voidp)&is, pngReadStream);
				loadPNG();
			}


			return olc::rcode::OK;


		fail_load:
			width = 0;
			height = 0;
			spr.clear();
			return olc::rcode::FAIL;
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | END IMAGE LOADER:                                                            |
// O------------------------------------------------------------------------------O
#pragma endregion
#pragma region stb
// O------------------------------------------------------------------------------O
// | START IMAGE LOADER: stb_image.h, all systems, very fast                      |
// O------------------------------------------------------------------------------O
// Thanks to Sean Barrett - https://github.com/nothings/stb/blob/master/stb_image.h
// MIT License - Copyright(c) 2017 Sean Barrett


// Note you need to download the above file into your project folder, and
// #define OLC_IMAGE_STB 
// #define OLC_PGE_APPLICATION
// #include "olcPixelGameEngine.h"


#if defined(OLC_IMAGE_STB)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace olc
{
	export class ImageLoader_STB : public olc::ImageLoader
	{
	public:
		ImageLoader_STB() : ImageLoader()
		{}


		std::vector<olc::Pixel>& LoadImageResource(const std::string& sImageFile, olc::ResourcePack* pack) override
		{
			// Open file
			stbi_uc* bytes = nullptr;
			int w = 0, h = 0, cmp = 0;
			if (pack != nullptr)
			{
				ResourceBuffer rb = pack->GetFileBuffer(sImageFile);
				bytes = stbi_load_from_memory((unsigned char*)rb.vMemory.data(), rb.vMemory.size(), &w, &h, &cmp, 4);
			}
			else
			{
				// Check file exists
				if (!_gfs::exists(sImageFile)) return olc::rcode::NO_FILE;
				bytes = stbi_load(sImageFile.c_str(), &w, &h, &cmp, 4);
			}

			std::vector<std::vector<olc::Pixel>> spr(h, std::vector<olc::Pixel>(w));

			if (!bytes) return olc::rcode::FAIL;
			for (int y = 1; y <= h; y++)
			{
				for (int x = 1; x <= w; x++)
				{
					olc::Pixel col;
					for (int i = 0; i < 4; i++)
					{
						col[i] = bytes[(x * y) * 4 + i]
					}
					spr[y][x] = col;
				}
			}
			delete[] bytes;
			return olc::rcode::OK;
		}
	};
}
#endif
// O------------------------------------------------------------------------------O
// | START IMAGE LOADER: stb_image.h                                              |
// O------------------------------------------------------------------------------O
#pragma endregion
#pragma endregion