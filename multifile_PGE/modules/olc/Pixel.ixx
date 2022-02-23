export module olc.Pixel;

import std.core;
import olc.xtra;

export namespace olc
{
    // O------------------------------------------------------------------------------O
    // | olc::Pixel - Represents a 32-Bit RGBA colour                                 |
    // O------------------------------------------------------------------------------O
    struct Pixel
    {
        union
        {
            uint32_t n;
            struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a; };
        };

        enum class Mode { NORMAL, MASK, ALPHA, CUSTOM };

        Pixel(uint32_t p = nDefaultPixel) : n(p) {}

        Pixel(int r, int g, int b, int a = nDefaultAlpha) :
            n(uint8_t(r) | (uint8_t(g) << 8) | (uint8_t(b) << 16) | (uint8_t(a) << 24))
        {} // Thanks jarekpelczar 

        explicit Pixel(float r, float g, float b, float a = 1.0f) :
            n(uint8_t(r * 255.0f) | (uint8_t(g * 255.0f) << 8) | (uint8_t(b * 255.0f) << 16) | (uint8_t(a * 255.0f) << 24))
        {}

        static olc::Pixel Lerp(const olc::Pixel& p1, const olc::Pixel& p2, float t)
        {
            return (p2 * t) + p1 * (1.0f - t);
        }

        olc::Pixel Lerp(const olc::Pixel& p2, float t)
        {
            return (p2 * t) + (*this) * (1.0f - t);
        }

        Pixel& operator = (const Pixel& v) = default;
        bool operator ==(const Pixel& p) const
        {
            return n == p.n;
        }

        bool operator !=(const Pixel& p) const
        {
            return n != p.n;
        }

        Pixel operator * (const float i) const
        {
            float fR = std::min(255.0f, std::max(0.0f, float(r) * i));
            float fG = std::min(255.0f, std::max(0.0f, float(g) * i));
            float fB = std::min(255.0f, std::max(0.0f, float(b) * i));
            return Pixel(uint8_t(fR), uint8_t(fG), uint8_t(fB), a);
        }

        Pixel operator / (const float i) const
        {
            float fR = std::min(255.0f, std::max(0.0f, float(r) / i));
            float fG = std::min(255.0f, std::max(0.0f, float(g) / i));
            float fB = std::min(255.0f, std::max(0.0f, float(b) / i));
            return Pixel(uint8_t(fR), uint8_t(fG), uint8_t(fB), a);
        }

        Pixel& operator *=(const float i)
        {
            this->r = uint8_t(std::min(255.0f, std::max(0.0f, float(r) * i)));
            this->g = uint8_t(std::min(255.0f, std::max(0.0f, float(g) * i)));
            this->b = uint8_t(std::min(255.0f, std::max(0.0f, float(b) * i)));
            return *this;
        }

        Pixel& operator /=(const float i)
        {
            this->r = uint8_t(std::min(255.0f, std::max(0.0f, float(r) / i)));
            this->g = uint8_t(std::min(255.0f, std::max(0.0f, float(g) / i)));
            this->b = uint8_t(std::min(255.0f, std::max(0.0f, float(b) / i)));
            return *this;
        }

        Pixel operator + (const Pixel& p) const
        {
            uint8_t nR = uint8_t(std::min(255, std::max(0, int(r) + int(p.r))));
            uint8_t nG = uint8_t(std::min(255, std::max(0, int(g) + int(p.g))));
            uint8_t nB = uint8_t(std::min(255, std::max(0, int(b) + int(p.b))));
            return Pixel(nR, nG, nB, a);
        }

        Pixel operator - (const Pixel& p) const
        {
            uint8_t nR = uint8_t(std::min(255, std::max(0, int(r) - int(p.r))));
            uint8_t nG = uint8_t(std::min(255, std::max(0, int(g) - int(p.g))));
            uint8_t nB = uint8_t(std::min(255, std::max(0, int(b) - int(p.b))));
            return Pixel(nR, nG, nB, a);
        }

        Pixel& operator +=(const Pixel& p)
        {
            this->r = uint8_t(std::min(255, std::max(0, int(r) + int(p.r))));
            this->g = uint8_t(std::min(255, std::max(0, int(g) + int(p.g))));
            this->b = uint8_t(std::min(255, std::max(0, int(b) + int(p.b))));
            return *this;
        }

        Pixel& operator -=(const Pixel& p) // Thanks Au Lit
        {
            this->r = uint8_t(std::min(255, std::max(0, int(r) - int(p.r))));
            this->g = uint8_t(std::min(255, std::max(0, int(g) - int(p.g))));
            this->b = uint8_t(std::min(255, std::max(0, int(b) - int(p.b))));
            return *this;
        }

        uint8_t operator [](size_t i)
        {
            i = i > 3 ? 3 : (i < 0 ? 0 : i);
            return *((uint8_t*)this + i);
        }

        Pixel inv() const
        {
            uint8_t nR = uint8_t(std::min(255, std::max(0, 255 - int(r))));
            uint8_t nG = uint8_t(std::min(255, std::max(0, 255 - int(g))));
            uint8_t nB = uint8_t(std::min(255, std::max(0, 255 - int(b))));
            return Pixel(nR, nG, nB, a);
        }
    };

    // O------------------------------------------------------------------------------O
    // | USEFUL CONSTANTS                                                             |
    // O------------------------------------------------------------------------------O
    const olc::Pixel
          GREY(192, 192, 192),  DARK_GREY(128, 128, 128),  VERY_DARK_GREY(64, 64, 64),
          RED(255, 0, 0),       DARK_RED(128, 0, 0),       VERY_DARK_RED(64, 0, 0),
          YELLOW(255, 255, 0),  DARK_YELLOW(128, 128, 0),  VERY_DARK_YELLOW(64, 64, 0),
          GREEN(0, 255, 0),     DARK_GREEN(0, 128, 0),     VERY_DARK_GREEN(0, 64, 0),
          CYAN(0, 255, 255),    DARK_CYAN(0, 128, 128),    VERY_DARK_CYAN(0, 64, 64),
          BLUE(0, 0, 255),      DARK_BLUE(0, 0, 128),      VERY_DARK_BLUE(0, 0, 64),
          MAGENTA(255, 0, 255), DARK_MAGENTA(128, 0, 128), VERY_DARK_MAGENTA(64, 0, 64),
          WHITE(255, 255, 255), BLACK(0, 0, 0),            BLANK(0, 0, 0, 0);
}