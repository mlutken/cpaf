#include <iomanip>
#include <sstream>
#include <boost/uuid/detail/sha1.hpp>
#include <boost/uuid/detail/md5.hpp>
#include "crypto_hash.h"
#include "md5sum.h"

namespace cpaf::crypto {


// See https://gist.github.com/qnighy/602bd3e0da381b89d08a216863fdc890

// http://www.zedwood.com/article/cpp-md5-function



/** Calculate sha1 from string and return as hex number string.
@return Hexadecimal string representation of the hash.
*/
std::string sha1(const std::string& text)
{
    boost::uuids::detail::sha1 sha1;
    unsigned char hash[20];
    sha1.process_bytes(text.c_str(), text.size());
    sha1.get_digest(hash);


    std::stringstream ss;
    ss << std::setfill('0') << std::hex;
    for(std::size_t i=0; i<sizeof(hash)/sizeof(hash[0]); ++i) {
        ss << std::setw(sizeof(hash[0])*2) << static_cast<uint16_t>(hash[i]);
    }
    return ss.str();
}


std::string md5(const std::string_view& text)
{
    md5sum md_sum;
    md_sum.write(text);
    return md_sum.sum_hex();
}

// --------------------
// --- CLASS md5sum ---
// --------------------

} //END namespace cpaf::crypto


