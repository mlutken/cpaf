#ifndef CIRCLEDETECT_H
#define CIRCLEDETECT_H

#include <iostream>
#include <sstream>
#include <primitives2d/circle.hpp>
#include <Color.h>

namespace detect {



template
<
      typename PixelT
    , class GetPixelPolicy
    , class EdgeDetectPolicy
>
class CircleDetect
{
public:
    // -----------------------------------
    // --- Constructors/init functions ---
    // -----------------------------------
    explicit CircleDetect( gmath::p2d::circle2f& c )
        : m_circle      ( c                         )
        , m_angleStart  ( 0                         )
        , m_angleEnd    ( gmath::Cf::PI()*2.0f      )
    {

    }

    // ----------------------
    // --- Work functions ---
    // ----------------------
    float   detect  ()
    {
        PixelT p1 = getPixel(0,0);
        PixelT p2 = getPixel(0,0);

        std::cout << "detect: " << detectEdge(p1,p2) << std::endl;
        std::cout << "  " << p1 << "  ,  " << p2 << std::endl;
    }

    PixelT      getPixel    ( int x, int y) const { return m_getPixelPolicy.get_pixel (x, y); }
    bool        detectEdge  ( const PixelT& p1, const PixelT& p2) const { return m_edgeDetectPolicy.detect (p1, p2); }

    // ----------------------
    // --- Info functions ---
    // ----------------------
    float     angleWork     () const    { return m_angleEnd - m_angleStart;  }

    // --------------------------------
    // --- Simple set/get functions ---
    // --------------------------------
    const gmath::p2d::circle2f&     circle          () const                    { return m_circle;  }
    void                            circleSet       ( gmath::p2d::circle2f& c)  { m_circle = c;     }
    float                           angleStart      () const                    { return m_angleStart; }
    void                            angleStartSet   ( float angle )             { m_angleStart = angle; }
    float                           angleEnd        () const                    { return m_angleEnd; }
    void                            angleEndSet     ( float angle )             { m_angleEnd = angle; }

    void                            getPixelPolicySet   ( const GetPixelPolicy& policy )    { m_getPixelPolicy = policy; }
    GetPixelPolicy&                 getPixelPolicy      ()                                  { return m_getPixelPolicy; }
    void                            edgeDetectPolicySet ( const GetPixelPolicy policy )     { m_getPixelPolicy = policy; }
    EdgeDetectPolicy&               edgeDetectPolicy    ()                                  { return m_getPixelPolicy; }
private:
    gmath::p2d::circle2f    m_circle;
    float                   m_angleStart;
    float                   m_angleEnd;
    GetPixelPolicy          m_getPixelPolicy;
    EdgeDetectPolicy        m_edgeDetectPolicy;

};

// ----------------------
// --- Free functions ---
// ----------------------

/** '<<' operator, Write circle detector (string) stream.
    \return Reference to (modified) ostream. */
template
<
    typename PixelT
    , class GetPixelPolicy
    , class EdgeDetectPolicy
>
inline std::ostream&
    operator <<(std::ostream& os,                               ///< [out] Output stream to write to.
                const CircleDetect<PixelT, GetPixelPolicy, EdgeDetectPolicy>& cd)	///< [in] Circle detector to write.
{
    os << cd.circle() << " angle:[" << cd.angleStart() << ", " << cd.angleEnd() << "]";
    return os;
}


} // END namespace detect

#endif // CIRCLEDETECT_H
