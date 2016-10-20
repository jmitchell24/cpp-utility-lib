#pragma once

#include "../String.hpp"
#include "../Math.hpp"

#define BYTE_MAX 255

namespace util
{
    struct ColorRGBA
    {
        typedef std::uint32_t Value;
        typedef std::uint8_t  Byte;
        typedef real_t        Real;
        typedef Vec3N<Byte>   Color3B;
        typedef Vec4N<Byte>   Color4B;
        typedef Vec3N<Real>   Color3R;
        typedef Vec4N<Real>   Color4R;

        typedef best_param<Color3B> Color3BParam;
        typedef best_param<Color4B> Color4BParam;
        typedef best_param<Color3R> Color3RParam;
        typedef best_param<Color4R> Color4RParam;

        union
        {
            struct { Byte r, g, b, a; };
            Byte components[4];
            Value value;
        };

        inline constexpr          ColorRGBA()                                                       : value()      {}
        inline constexpr explicit ColorRGBA(Value value)                                            : value(value) {}
        inline constexpr          ColorRGBA(Byte const r, Byte const g, Byte const b)               : components{r, g, b}    {}
        inline constexpr          ColorRGBA(Byte const r, Byte const g, Byte const b, Byte const a) : components{r, g, b, a} {}
        inline constexpr          ColorRGBA(Real const r, Real const g, Real const b)               : components{(Byte)(r*BYTE_MAX), (Byte)(g*BYTE_MAX), (Byte)(b*BYTE_MAX), Byte()}             {}
        inline constexpr          ColorRGBA(Real const r, Real const g, Real const b, Real const a) : components{(Byte)(r*BYTE_MAX), (Byte)(g*BYTE_MAX), (Byte)(b*BYTE_MAX), (Byte)(a*BYTE_MAX)} {}

        explicit ColorRGBA(Color3BParam c);
        explicit ColorRGBA(Color4BParam c);
        explicit ColorRGBA(Color3RParam c);
        explicit ColorRGBA(Color4RParam c);

        operator Color3B();
        operator Color4B();
        operator Color3R();
        operator Color4R();

        string_t str() const;
        bool tryParse(Segment const& s);
        bool tryParse(istream_t& is);

        DEFINE_OBJECT_OSTREAM_OPERATOR(ColorRGBA)
        DEFINE_OBJECT_ISTREAM_OPERATOR(ColorRGBA)
    };

    inline constexpr ColorRGBA colorByte(ColorRGBA::Byte const r, ColorRGBA::Byte const g, ColorRGBA::Byte const b) { return ColorRGBA(r,g,b); }
    inline constexpr ColorRGBA colorReal(ColorRGBA::Real const r, ColorRGBA::Real const g, ColorRGBA::Real const b) { return ColorRGBA(r,g,b); }
    inline constexpr ColorRGBA colorByte(ColorRGBA::Byte const r, ColorRGBA::Byte const g, ColorRGBA::Byte const b, ColorRGBA::Byte const a) { return ColorRGBA(r,g,b,a); }
    inline constexpr ColorRGBA colorReal(ColorRGBA::Real const r, ColorRGBA::Real const g, ColorRGBA::Real const b, ColorRGBA::Real const a) { return ColorRGBA(r,g,b,a); }

namespace colors
{ // x11 standard colors
    ::util::ColorRGBA constexpr static AliceBlue = ::util::colorByte( 240,248,255,255 );
    ::util::ColorRGBA constexpr static AntiqueWhite = ::util::colorByte( 250,235,215,255 );
    ::util::ColorRGBA constexpr static Aqua = ::util::colorByte( 0,255,255,255 );
    ::util::ColorRGBA constexpr static Aquamarine = ::util::colorByte( 127,255,212,255 );
    ::util::ColorRGBA constexpr static Azure = ::util::colorByte( 240,255,255,255 );
    ::util::ColorRGBA constexpr static Beige = ::util::colorByte( 245,245,220,255 );
    ::util::ColorRGBA constexpr static Bisque = ::util::colorByte( 255,228,196,255 );
    ::util::ColorRGBA constexpr static Black = ::util::colorByte( 0,0,0,255 );
    ::util::ColorRGBA constexpr static BlanchedAlmond = ::util::colorByte( 255,235,205,255 );
    ::util::ColorRGBA constexpr static Blue = ::util::colorByte( 0,0,255,255 );
    ::util::ColorRGBA constexpr static BlueViolet = ::util::colorByte( 138,43,226,255 );
    ::util::ColorRGBA constexpr static Brown = ::util::colorByte( 165,42,42,255 );
    ::util::ColorRGBA constexpr static BurlyWood = ::util::colorByte( 222,184,135,255 );
    ::util::ColorRGBA constexpr static CadetBlue = ::util::colorByte( 95,158,160,255 );
    ::util::ColorRGBA constexpr static Chartreuse = ::util::colorByte( 127,255,0,255 );
    ::util::ColorRGBA constexpr static Chocolate = ::util::colorByte( 210,105,30,255 );
    ::util::ColorRGBA constexpr static Coral = ::util::colorByte( 255,127,80,255 );
    ::util::ColorRGBA constexpr static CornflowerBlue = ::util::colorByte( 100,149,237,255 );
    ::util::ColorRGBA constexpr static Cornsilk = ::util::colorByte( 255,248,220,255 );
    ::util::ColorRGBA constexpr static Crimson = ::util::colorByte( 220,20,60,255 );
    ::util::ColorRGBA constexpr static Cyan = ::util::colorByte( 0,255,255,255 );
    ::util::ColorRGBA constexpr static DarkBlue = ::util::colorByte( 0,0,139,255 );
    ::util::ColorRGBA constexpr static DarkCyan = ::util::colorByte( 0,139,139,255 );
    ::util::ColorRGBA constexpr static DarkGoldenrod = ::util::colorByte( 184,134,11,255 );
    ::util::ColorRGBA constexpr static DarkGray = ::util::colorByte( 169,169,169,255 );
    ::util::ColorRGBA constexpr static DarkGreen = ::util::colorByte( 0,100,0,255 );
    ::util::ColorRGBA constexpr static DarkKhaki = ::util::colorByte( 189,183,107,255 );
    ::util::ColorRGBA constexpr static DarkMagenta = ::util::colorByte( 139,0,139,255 );
    ::util::ColorRGBA constexpr static DarkOliveGreen = ::util::colorByte( 85,107,47,255 );
    ::util::ColorRGBA constexpr static DarkOrange = ::util::colorByte( 255,140,0,255 );
    ::util::ColorRGBA constexpr static DarkOrchid = ::util::colorByte( 153,50,204,255 );
    ::util::ColorRGBA constexpr static DarkRed = ::util::colorByte( 139,0,0,255 );
    ::util::ColorRGBA constexpr static DarkSalmon = ::util::colorByte( 233,150,122,255 );
    ::util::ColorRGBA constexpr static DarkSeaGreen = ::util::colorByte( 143,188,143,255 );
    ::util::ColorRGBA constexpr static DarkSlateBlue = ::util::colorByte( 72,61,139,255 );
    ::util::ColorRGBA constexpr static DarkSlateGray = ::util::colorByte( 47,79,79,255 );
    ::util::ColorRGBA constexpr static DarkTurquoise = ::util::colorByte( 0,206,209,255 );
    ::util::ColorRGBA constexpr static DarkViolet = ::util::colorByte( 148,0,211,255 );
    ::util::ColorRGBA constexpr static DeepPink = ::util::colorByte( 255,20,147,255 );
    ::util::ColorRGBA constexpr static DeepSkyBlue = ::util::colorByte( 0,191,255,255 );
    ::util::ColorRGBA constexpr static DimGray = ::util::colorByte( 105,105,105,255 );
    ::util::ColorRGBA constexpr static DodgerBlue = ::util::colorByte( 30,144,255,255 );
    ::util::ColorRGBA constexpr static FireBrick = ::util::colorByte( 178,34,34,255 );
    ::util::ColorRGBA constexpr static FloralWhite = ::util::colorByte( 255,250,240,255 );
    ::util::ColorRGBA constexpr static ForestGreen = ::util::colorByte( 34,139,34,255 );
    ::util::ColorRGBA constexpr static Fuchsia = ::util::colorByte( 255,0,255,255 );
    ::util::ColorRGBA constexpr static Gainsboro = ::util::colorByte( 220,220,220,255 );
    ::util::ColorRGBA constexpr static GhostWhite = ::util::colorByte( 248,248,255,255 );
    ::util::ColorRGBA constexpr static Gold = ::util::colorByte( 255,215,0,255 );
    ::util::ColorRGBA constexpr static Goldenrod = ::util::colorByte( 218,165,32,255 );
    ::util::ColorRGBA constexpr static Gray = ::util::colorByte( 128,128,128,255 );
    ::util::ColorRGBA constexpr static Green = ::util::colorByte( 0,128,0,255 );
    ::util::ColorRGBA constexpr static GreenYellow = ::util::colorByte( 173,255,47,255 );
    ::util::ColorRGBA constexpr static Honeydew = ::util::colorByte( 240,255,240,255 );
    ::util::ColorRGBA constexpr static HotPink = ::util::colorByte( 255,105,180,255 );
    ::util::ColorRGBA constexpr static IndianRed = ::util::colorByte( 205,92,92,255 );
    ::util::ColorRGBA constexpr static Indigo = ::util::colorByte( 75,0,130,255 );
    ::util::ColorRGBA constexpr static Ivory = ::util::colorByte( 255,255,240,255 );
    ::util::ColorRGBA constexpr static Khaki = ::util::colorByte( 240,230,140,255 );
    ::util::ColorRGBA constexpr static Lavender = ::util::colorByte( 230,230,250,255 );
    ::util::ColorRGBA constexpr static LavenderBlush = ::util::colorByte( 255,240,245,255 );
    ::util::ColorRGBA constexpr static LawnGreen = ::util::colorByte( 124,252,0,255 );
    ::util::ColorRGBA constexpr static LemonChiffon = ::util::colorByte( 255,250,205,255 );
    ::util::ColorRGBA constexpr static LightBlue = ::util::colorByte( 173,216,230,255 );
    ::util::ColorRGBA constexpr static LightCoral = ::util::colorByte( 240,128,128,255 );
    ::util::ColorRGBA constexpr static LightCyan = ::util::colorByte( 224,255,255,255 );
    ::util::ColorRGBA constexpr static LightGoldenrodYellow = ::util::colorByte( 250,250,210,255 );
    ::util::ColorRGBA constexpr static LightGreen = ::util::colorByte( 144,238,144,255 );
    ::util::ColorRGBA constexpr static LightGrey = ::util::colorByte( 211,211,211,255 );
    ::util::ColorRGBA constexpr static LightPink = ::util::colorByte( 255,182,193,255 );
    ::util::ColorRGBA constexpr static LightSalmon = ::util::colorByte( 255,160,122,255 );
    ::util::ColorRGBA constexpr static LightSeaGreen = ::util::colorByte( 32,178,170,255 );
    ::util::ColorRGBA constexpr static LightSkyBlue = ::util::colorByte( 135,206,250,255 );
    ::util::ColorRGBA constexpr static LightSlateGray = ::util::colorByte( 119,136,153,255 );
    ::util::ColorRGBA constexpr static LightSteelBlue = ::util::colorByte( 176,196,222,255 );
    ::util::ColorRGBA constexpr static LightYellow = ::util::colorByte( 255,255,224,255 );
    ::util::ColorRGBA constexpr static Lime = ::util::colorByte( 0,255,0,255 );
    ::util::ColorRGBA constexpr static LimeGreen = ::util::colorByte( 50,205,50,255 );
    ::util::ColorRGBA constexpr static Linen = ::util::colorByte( 250,240,230,255 );
    ::util::ColorRGBA constexpr static Magenta = ::util::colorByte( 255,0,255,255 );
    ::util::ColorRGBA constexpr static Maroon = ::util::colorByte( 128,0,0,255 );
    ::util::ColorRGBA constexpr static MediumAquamarine = ::util::colorByte( 102,205,170,255 );
    ::util::ColorRGBA constexpr static MediumBlue = ::util::colorByte( 0,0,205,255 );
    ::util::ColorRGBA constexpr static MediumOrchid = ::util::colorByte( 186,85,211,255 );
    ::util::ColorRGBA constexpr static MediumPurple = ::util::colorByte( 147,112,219,255 );
    ::util::ColorRGBA constexpr static MediumSeaGreen = ::util::colorByte( 60,179,113,255 );
    ::util::ColorRGBA constexpr static MediumSlateBlue = ::util::colorByte( 123,104,238,255 );
    ::util::ColorRGBA constexpr static MediumSpringGreen = ::util::colorByte( 0,250,154,255 );
    ::util::ColorRGBA constexpr static MediumTurquoise = ::util::colorByte( 72,209,204,255 );
    ::util::ColorRGBA constexpr static MediumVioletRed = ::util::colorByte( 199,21,133,255 );
    ::util::ColorRGBA constexpr static MidnightBlue = ::util::colorByte( 25,25,112,255 );
    ::util::ColorRGBA constexpr static MintCream = ::util::colorByte( 245,255,250,255 );
    ::util::ColorRGBA constexpr static MistyRose = ::util::colorByte( 255,228,225,255 );
    ::util::ColorRGBA constexpr static Moccasin = ::util::colorByte( 255,228,181,255 );
    ::util::ColorRGBA constexpr static NavajoWhite = ::util::colorByte( 255,222,173,255 );
    ::util::ColorRGBA constexpr static Navy = ::util::colorByte( 0,0,128,255 );
    ::util::ColorRGBA constexpr static OldLace = ::util::colorByte( 253,245,230,255 );
    ::util::ColorRGBA constexpr static Olive = ::util::colorByte( 128,128,0,255 );
    ::util::ColorRGBA constexpr static OliveDrab = ::util::colorByte( 107,142,35,255 );
    ::util::ColorRGBA constexpr static Orange = ::util::colorByte( 255,165,0,255 );
    ::util::ColorRGBA constexpr static OrangeRed = ::util::colorByte( 255,69,0,255 );
    ::util::ColorRGBA constexpr static Orchid = ::util::colorByte( 218,112,214,255 );
    ::util::ColorRGBA constexpr static PaleGoldenrod = ::util::colorByte( 238,232,170,255 );
    ::util::ColorRGBA constexpr static PaleGreen = ::util::colorByte( 152,251,152,255 );
    ::util::ColorRGBA constexpr static PaleTurquoise = ::util::colorByte( 175,238,238,255 );
    ::util::ColorRGBA constexpr static PaleVioletRed = ::util::colorByte( 219,112,147,255 );
    ::util::ColorRGBA constexpr static PapayaWhip = ::util::colorByte( 255,239,213,255 );
    ::util::ColorRGBA constexpr static PeachPuff = ::util::colorByte( 255,218,185,255 );
    ::util::ColorRGBA constexpr static Peru = ::util::colorByte( 205,133,63,255 );
    ::util::ColorRGBA constexpr static Pink = ::util::colorByte( 255,192,203,255 );
    ::util::ColorRGBA constexpr static Plum = ::util::colorByte( 221,160,221,255 );
    ::util::ColorRGBA constexpr static PowderBlue = ::util::colorByte( 176,224,230,255 );
    ::util::ColorRGBA constexpr static Purple = ::util::colorByte( 128,0,128,255 );
    ::util::ColorRGBA constexpr static Red = ::util::colorByte( 255,0,0,255 );
    ::util::ColorRGBA constexpr static RosyBrown = ::util::colorByte( 188,143,143,255 );
    ::util::ColorRGBA constexpr static RoyalBlue = ::util::colorByte( 65,105,225,255 );
    ::util::ColorRGBA constexpr static SaddleBrown = ::util::colorByte( 139,69,19,255 );
    ::util::ColorRGBA constexpr static Salmon = ::util::colorByte( 250,128,114,255 );
    ::util::ColorRGBA constexpr static SandyBrown = ::util::colorByte( 244,164,96,255 );
    ::util::ColorRGBA constexpr static SeaGreen = ::util::colorByte( 46,139,87,255 );
    ::util::ColorRGBA constexpr static Seashell = ::util::colorByte( 255,245,238,255 );
    ::util::ColorRGBA constexpr static Sienna = ::util::colorByte( 160,82,45,255 );
    ::util::ColorRGBA constexpr static Silver = ::util::colorByte( 192,192,192,255 );
    ::util::ColorRGBA constexpr static SkyBlue = ::util::colorByte( 135,206,235,255 );
    ::util::ColorRGBA constexpr static SlateBlue = ::util::colorByte( 106,90,205,255 );
    ::util::ColorRGBA constexpr static SlateGray = ::util::colorByte( 112,128,144,255 );
    ::util::ColorRGBA constexpr static Snow = ::util::colorByte( 255,250,250,255 );
    ::util::ColorRGBA constexpr static SpringGreen = ::util::colorByte( 0,255,127,255 );
    ::util::ColorRGBA constexpr static SteelBlue = ::util::colorByte( 70,130,180,255 );
    ::util::ColorRGBA constexpr static Tan = ::util::colorByte( 210,180,140,255 );
    ::util::ColorRGBA constexpr static Teal = ::util::colorByte( 0,128,128,255 );
    ::util::ColorRGBA constexpr static Thistle = ::util::colorByte( 216,191,216,255 );
    ::util::ColorRGBA constexpr static Tomato = ::util::colorByte( 255,99,71,255 );
    ::util::ColorRGBA constexpr static Turquoise = ::util::colorByte( 64,224,208,255 );
    ::util::ColorRGBA constexpr static Violet = ::util::colorByte( 238,130,238,255 );
    ::util::ColorRGBA constexpr static Wheat = ::util::colorByte( 245,222,179,255 );
    ::util::ColorRGBA constexpr static White = ::util::colorByte( 255,255,255,255 );
    ::util::ColorRGBA constexpr static WhiteSmoke = ::util::colorByte( 245,245,245,255 );
    ::util::ColorRGBA constexpr static Yellow = ::util::colorByte( 255,255,0,255 );
    ::util::ColorRGBA constexpr static YellowGreen = ::util::colorByte( 154,205,50,255 );
}
}
