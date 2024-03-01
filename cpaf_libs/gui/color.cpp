#include "color.h"



namespace cpaf::gui{


u8color color::to_u8color() const
{
    return u8color(r()*255, g()*255, b()*255, a()*255);
}


color::color(nlohmann::json jo_color)
{
    if (jo_color.is_array()) {
        unsigned i = 0;
        for (auto jo : jo_color.items()) {
            if (i > 3) {
                break;
            }
            (*this)[i] = cpaf::json_value_float(jo, 0);
        }
    }
}



} //END namespace cpaf::gui

