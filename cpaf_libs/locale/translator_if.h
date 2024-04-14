#pragma once

#include <string>


namespace cpaf::locale {


/**

*/
class translator_if
{
public:
    virtual ~translator_if() = default;

    std::string             tr              (const std::string& text) const { return do_translate(text); }

    std::string             target_language () const { return do_target_language(); }
    std::string             id              () const { return do_id(); }


private:
    virtual std::string     do_translate        (const std::string& text) const;
    virtual std::string     do_target_language  () const = 0;
    virtual std::string     do_id               () const;

};



} //END namespace cpaf::locale




