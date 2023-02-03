#ifndef COLOR_PIXEL_GET_POLICIES_H
#define COLOR_PIXEL_GET_POLICIES_H

#include <iostream>
#include <Color.h>

namespace detect {

class GetDummyColorPixelPolicy
{
public:
//    typedef detect::Color pixel_type;
    typedef detect::Color           value_type;
    typedef detect::Color&          reference;
    typedef const detect::Color&    const_reference;
    typedef detect::Color*          pointer;
    typedef const detect::Color*    const_pointer;
    typedef size_t                  size_type;
    typedef int                     difference_type;

    GetDummyColorPixelPolicy() {}
    Color       get_pixel (int x, int y) const
    {
        return Color(0,255,0,255);
    }

    Color&      pixel_ref (int x, int y)
    {
        return m_pixel_ref_dummy;
    }

    void       set_pixel (int x, int y, const Color& color) const
    {
        std::cout << "set_pixel: (" << x << "," << y << ") => " << color << std::endl;
    }

private:
    Color m_pixel_ref_dummy;
};


} // END namespace detect

#endif // COLOR_PIXEL_GET_POLICIES_H


