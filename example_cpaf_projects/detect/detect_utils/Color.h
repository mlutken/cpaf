#ifndef COLOR_H
#define COLOR_H

#include <cstring>
#include <iostream>
#include <boost/cstdint.hpp>

namespace detect {


class Color
{
public:
    Color() {}
    explicit Color  (boost::uint32_t colorUint32 )
        : m_uint32(colorUint32) {}

    Color (boost::uint8_t red, boost::uint8_t green, boost::uint8_t blue)
    {
        m_rgba8.r = red;
        m_rgba8.g = green;
        m_rgba8.b = blue;

    }

    Color (boost::uint8_t red, boost::uint8_t green, boost::uint8_t blue, boost::uint8_t alpha)
    {
        m_rgba8.r = red;
        m_rgba8.g = green;
        m_rgba8.b = blue;
        m_rgba8.a = alpha;
    }

    boost::uint8_t          red     () const { return m_rgba8.r; }
    boost::uint8_t          green   () const { return m_rgba8.g; }
    boost::uint8_t          blue    () const { return m_rgba8.b; }
    boost::uint8_t          alpha   () const { return m_rgba8.a; }
    const boost::uint8_t*   array8  () const { return m_array8; }

    void  redSet            ( boost::uint8_t  r ) { m_rgba8.r = r; }
    void  greenSet          ( boost::uint8_t  g ) { m_rgba8.g = g; }
    void  blueSet           ( boost::uint8_t  b ) { m_rgba8.b = b; }
    void  alphaSet          ( boost::uint8_t  a ) { m_rgba8.a = a; }
    void array8Set          ( const boost::uint8_t array8[4] ) { std::memcpy( m_array8, array8, 4); }

private:
    struct RGBA8 {
        boost::uint8_t  r;
        boost::uint8_t  g;
        boost::uint8_t  b;
        boost::uint8_t  a;
    };

    union {
        RGBA8               m_rgba8;
        boost::uint32_t     m_uint32;
        boost::uint8_t      m_array8[4];
    };
};

/** '<<' operator, Write Color to (string) stream.
    \return Reference to (modified) ostream. */
inline std::ostream&
    operator <<(std::ostream& os,   ///< [out] Output stream to write to.
                const Color& col)	///< [in] Color to write.
{
    os << "rgba: ("
       << static_cast<int>(col.red())   << ","
       << static_cast<int>(col.green()) << ","
       << static_cast<int>(col.blue())  << ","
       << static_cast<int>(col.alpha()) << ")"
        ;
    return os;
}


} // END namespace detect

#endif // COLOR_H
