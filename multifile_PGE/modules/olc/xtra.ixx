export module olc.xtra;

import <stdint.h>;

export namespace olc
{
	// Pixel Game Engine Advanced Configuration
	constexpr uint8_t  nMouseButtons = 5;
	constexpr uint8_t  nDefaultAlpha = 0xFF;
	constexpr uint32_t nDefaultPixel = (nDefaultAlpha << 24);
	constexpr uint8_t  nTabSizeInSpaces = 4;
	enum class rcode { FAIL = 0, OK = 1, NO_FILE = -1 };
}