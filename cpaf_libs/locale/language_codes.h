#pragma once

#include <nlohmann/json.hpp>

namespace cpaf::locale {
class translator;

/**

@see https://gist.github.com/Josantonius/b455e315bc7f790d14b136d61d9ae469
*/
class language_codes
{
public:

    static std::string                      language_name       (const std::string& language_code_lookup, const translator& tr);
    static std::string                      language_name       (const std::string& language_code_lookup);
    static bool                             is_languge_code     (const std::string& language_code_lookup);
    static const nlohmann::json&            codes_and_names     ();


};



} //END namespace cpaf::locale




