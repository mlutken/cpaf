#include "color.h"



namespace cpaf::gui{

u8color color::to_u8color() const
{
    return u8color(r()*255, g()*255, b()*255, a()*255);
}





} //END namespace cpaf::gui

