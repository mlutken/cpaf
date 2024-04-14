#pragma once


#include <cpaf_libs/locale/translator_if.h>

namespace cpaf::locale {


/**

@see https://gist.github.com/Josantonius/b455e315bc7f790d14b136d61d9ae469
*/
class ui_translator: public translator_if
{
public:
    using translator_if::translator_if;	// "Import" constructors to scope

private:
    std::string     do_translate        (const std::string& text) const override;
    std::string     do_target_language  () const override { return target_language_; }

    std::string     target_language_ = "dev";

};



} //END namespace cpaf::locale




