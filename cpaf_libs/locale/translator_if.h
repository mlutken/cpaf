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

private:
    virtual std::string     do_translate    (const std::string& text) const;

};



} //END namespace cpaf::locale




