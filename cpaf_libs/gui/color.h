#pragma once

#include <cstdint>
#include <string>
#include <cpaf_libs/math/base/v4.hpp>

namespace cpaf::gui {

/** \unit color.h
 *  Color types for Cpaf Gui
\author Martin Lutken
*/


class u8color;

class color : public cpaf::math::v4f
{
public:
    using cpaf::math::v4f::v4f; // "Import" constructors to scope

    static color   rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return color(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
    }

    color   (float r, float g, float b, float a) : cpaf::math::v4f(r, g, b, a) {}

    float           r               () const        { return x(); }
    float&          r               ()              { return x(); }
    float           g               () const        { return y(); }
    float&          g               ()              { return y(); }
    float           b               () const        { return z(); }
    float&          b               ()              { return z(); }
    float           a               () const        { return w(); }
    float&          a               ()              { return w(); }

    u8color         to_u8color      () const;
};

using v4u8 = cpaf::math::v4<uint8_t>;

class u8color : public v4u8
{
public:
    using v4u8::v4u8; // "Import" constructors to scope

    u8color   (uint8_t r, uint8_t g, uint8_t b, uint8_t a) : v4u8(r, g, b, a) {}

    uint8_t         r               () const        { return x(); }
    uint8_t&        r               ()              { return x(); }
    uint8_t         g               () const        { return y(); }
    uint8_t&        g               ()              { return y(); }
    uint8_t         b               () const        { return z(); }
    uint8_t&        b               ()              { return z(); }
    uint8_t         a               () const        { return w(); }
    uint8_t&        a               ()              { return w(); }

    color           to_color        () const { return color(r()/255.0f, g()/255.0f, b()/255.0f, a()/255.0f); }
};



//std::string to_string   (audio_format_t audio_format);

} //END namespace cpaf::gui

