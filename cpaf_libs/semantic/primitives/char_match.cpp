#include <iostream>
#include <unicode/cpaf_u16string_utils.h>
#include "char_match.h"

using namespace cpaf::unicode::osops;
using namespace cpaf::unicode;
using namespace std;

namespace cpaf::semantic
{

char_match_factory& char_match_factory::i()
{
    static char_match_factory fac;
    return fac;
}

char_match_base::~char_match_base()
{

}

//std::unique_ptr<char_match_base> char_match_factory::create(const u16string& match) const
//{

//}

//std::unique_ptr<char_match_base> char_match_factory::create_simple(const u16string_view& sub_match) const
//{

//}

} //end namespace cpaf::semantic
