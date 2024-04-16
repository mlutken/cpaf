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

    static std::string                      language_name       (const std::string& iso639_2_language_code, const translator& tr);
    static std::string                      language_name       (const std::string& iso639_2_language_code);
    static bool                             is_languge_code     (const std::string& iso639_2_language_code);
    static const nlohmann::json&            codes_and_names     ();
    static std::string                      iso639_3_to_2       (const std::string& iso639_3);



};



} //END namespace cpaf::locale




