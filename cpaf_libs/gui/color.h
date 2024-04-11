#pragma once

#include <cstdint>
#include <string>
#include <cpaf_libs/math/base/v4.hpp>
#include <cpaf_libs/system/cpaf_system_info.h>
#include <nlohmann/json.hpp>


namespace cpaf::gui {

/** \unit color.h
 *  Color types for Cpaf Gui
\author Martin Lutken
*/


class u8color;

class color : public cpaf::math::v4f
{
public:

    constexpr static color default_val() { return color(0,0,0,0); }
    using cpaf::math::v4f::v4f; // "Import" constructors to scope

    constexpr static color   rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return color(r/255.0f, g/255.0f, b/255.0f, a/255.0f);
    }

    constexpr static color   rgba(uint32_t rgba) {
        if constexpr (cpaf::system::is_little_endian()) {
            return color(((rgba >> 24) & 0xff) / 255.0f,
                         ((rgba >> 16) & 0xff) / 255.0f,
                         ((rgba >> 8)  & 0xff) / 255.0f,
                         ((rgba >> 0)  & 0xff) / 255.0f
                         );
        }
        else {
            return color(((rgba >> 0)  & 0xff) / 255.0f,
                         ((rgba >> 8)  & 0xff) / 255.0f,
                         ((rgba >> 16) & 0xff) / 255.0f,
                         ((rgba >> 24) & 0xff) / 255.0f
                         );
        }
    }

    constexpr color   (float r, float g, float b, float a) : cpaf::math::v4f(r, g, b, a) {}
    explicit color   (nlohmann::json jo_color);

    /// Implicit conversion from v4f to color!
    /// @todo Do we really want this?
    constexpr color   (cpaf::math::v4f v) : cpaf::math::v4f(v) {}

    nlohmann::json  to_json         () const        { return nlohmann::json::array({r(),g(),b(),a()}); }

    constexpr float           r               () const        { return x(); }
    constexpr float&          r               ()              { return x(); }
    constexpr float           g               () const        { return y(); }
    constexpr float&          g               ()              { return y(); }
    constexpr float           b               () const        { return z(); }
    constexpr float&          b               ()              { return z(); }
    constexpr float           a               () const        { return w(); }
    constexpr float&          a               ()              { return w(); }

    u8color         to_u8color      () const;
};

using v4u8 = cpaf::math::v4<uint8_t>;

class u8color : public v4u8
{
public:
    using v4u8::v4u8; // "Import" constructors to scope

    constexpr u8color   (uint8_t r, uint8_t g, uint8_t b, uint8_t a) : v4u8(r, g, b, a) {}

    constexpr uint8_t         r               () const        { return x(); }
    constexpr uint8_t&        r               ()              { return x(); }
    constexpr uint8_t         g               () const        { return y(); }
    constexpr uint8_t&        g               ()              { return y(); }
    constexpr uint8_t         b               () const        { return z(); }
    constexpr uint8_t&        b               ()              { return z(); }
    constexpr uint8_t         a               () const        { return w(); }
    constexpr uint8_t&        a               ()              { return w(); }

    constexpr color           to_color        () const { return color(r()/255.0f, g()/255.0f, b()/255.0f, a()/255.0f); }
};



//std::string to_string   (audio_format_t audio_format);

} //END namespace cpaf::gui

