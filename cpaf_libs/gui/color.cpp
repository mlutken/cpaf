#include "color.h"

#include <cpaf_libs/utils/cpaf_json_utils.h>

namespace cpaf::gui{


u8color color::to_u8color() const
{
    return u8color(r()*255, g()*255, b()*255, a()*255);
}


color::color(nlohmann::json jo_color)
{
    if (jo_color.is_array()) {
        unsigned i = 0;
        for (auto el : jo_color.items()) {
            if (i > 3) {
                break;
            }
            const float val = cpaf::json_value_float(el.value(), 0);
            // std::cerr << "FIXMENM jo: " << el.value() << "\n";
            // std::cerr << "FIXMENM val: " << val << "\n";
            (*this)[i] = val;
            ++i;
        }
    }
}



} //END namespace cpaf::gui

