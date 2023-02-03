#ifndef COLOR_EDGDE_DETECT_POLICIES_H
#define COLOR_EDGDE_DETECT_POLICIES_H

#include <Color.h>


namespace detect {


struct ColorEdgeDefaultPolicy
{
    ColorEdgeDefaultPolicy()
        : m_threshold(20)
    {
    }

    bool detect ( const Color& c1, const Color& c2) const
    {
        return  (m_threshold < std::abs(c1.red() - c2.red())    )   ||
                (m_threshold < std::abs(c1.green() - c2.green()))   ||
                (m_threshold < std::abs(c1.blue() - c2.blue())  )
                ;
    }

    const int m_threshold;
};

} // END namespace detect

#endif // COLOR_EDGDE_DETECT_POLICIES_H
