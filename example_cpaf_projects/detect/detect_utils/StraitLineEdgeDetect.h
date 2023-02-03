#ifndef STRAITLINEEDGEDETECT_H
#define STRAITLINEEDGEDETECT_H

#include <base/v2.hpp>

namespace detect {

/** */
class StraitLineEdgeDetect
{
public:
    StraitLineEdgeDetect();

private:
    gmath::v2f  m_lineStartPos;
    gmath::v2f  m_lineEndPos;

};

} // END namespace detect

#endif // STRAITLINEEDGEDETECT_H
