export module olc.Renderer;

import <stdint.h>;
import std.core;
import olc.xtra;
import olc.V2D;
import olc.Pixel;
import olc.Sprite;
import olc.PGE;

export namespace olc
{
	class Renderer
	{
	public:
		virtual ~Renderer() = default;
		virtual void       PrepareDevice() = 0;
		virtual olc::rcode CreateDevice(std::vector<void*> params, bool bFullScreen, bool bVSYNC) = 0;
		virtual olc::rcode DestroyDevice() = 0;
		virtual void       DisplayFrame() = 0;
		virtual void       PrepareDrawing() = 0;
		virtual void	   SetDecalMode(const olc::DecalMode& mode) = 0;
		virtual void       DrawLayerQuad(const olc::vf2d& offset, const olc::vf2d& scale, const olc::Pixel tint) = 0;
		virtual void       DrawDecal(const olc::DecalInstance& decal) = 0;
		virtual uint32_t   CreateTexture(const uint32_t width, const uint32_t height, const bool filtered = false, const bool clamp = true) = 0;
		virtual void       UpdateTexture(uint32_t id, olc::Sprite* spr) = 0;
		virtual void       ReadTexture(uint32_t id, olc::Sprite* spr) = 0;
		virtual uint32_t   DeleteTexture(const uint32_t id) = 0;
		virtual void       ApplyTexture(uint32_t id) = 0;
		virtual void       UpdateViewport(const olc::vi2d& pos, const olc::vi2d& size) = 0;
		virtual void       ClearBuffer(olc::Pixel p, bool bDepth) = 0;
		static olc::PixelGameEngine* ptrPGE;
	};

	olc::PixelGameEngine* olc::Renderer::ptrPGE = nullptr;
}