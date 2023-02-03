
#include "cpaf_string8.h"

namespace cpaf::containers
{

/** '<<' operator, Write string8 to stream.
    \return Reference to (modified) ostream. */
inline std::ostream& operator<< (std::ostream& os, const string8& str)
{
    os << str.c_str();
    return os;
}

} //end namespace cpaf::containers


