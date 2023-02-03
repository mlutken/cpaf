#ifndef CPAF_DEBUG_UTILS_H
#define CPAF_DEBUG_UTILS_H

#include <vector>
#include <map>
#include <string>
#include <sstream>


namespace cpaf::dbg {

/** Create a simple debug string from a dictionary type contaier */
template <class CONTAINER>
std::string dict_container_str(const CONTAINER& container)
{
    std::stringstream ss;
    for (const auto& kv_pair : container) {
        ss << kv_pair.first << ": " << kv_pair.second << "\n";
    }
    return ss.str();
}

} // END namespace cpaf::dbg

#endif //CPAF_DEBUG_UTILS_H

