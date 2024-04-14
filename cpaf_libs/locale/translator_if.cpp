
#include "translator_if.h"



namespace cpaf::locale {

std::string translator_if::do_translate(const std::string& text) const
{
    return text;
}

std::string translator_if::do_id() const
{
    return do_target_language();
}




} //END namespace cpaf::locale
