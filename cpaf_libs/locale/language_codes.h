#pragma once

#include <string>
#include <vector>
#include <map>


namespace cpaf::locale {


/**

@see https://gist.github.com/Josantonius/b455e315bc7f790d14b136d61d9ae469
*/
class language_codes
{
    using lc_name_map_t = std::map<std::string, std::string>;

    static const lc_name_map_t&             codes_and_names     ();
    static const std::vector<std::string>&  codes               ();

    static bool                             is_languge_code     (const std::string& language_code_lower_case);



};



} //END namespace cpaf::locale




