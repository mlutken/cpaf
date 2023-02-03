#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#include <streams/cpaf_json_streams.h>
#include <filesystem/cpaf_special_dirs.h>
#include <utils/cpaf_string_utils.hpp>

#include <unicode/cpaf_u16string_utils.h>
#include <semantic/readers/json_classification_data_reader.h>
#include <semantic/primitives/pattern_matcher.h>
#include <semantic/classifier/fragments_classifier.h>

using json = nlohmann::json;
namespace fs = boost::filesystem;
namespace cfs = cpaf::filesystem;
namespace cs = cpaf::streams;
namespace con = cpaf::containers;
namespace cu = cpaf::unicode;

using namespace std;



using namespace cpaf::unicode::osops;
using namespace cpaf::semantic;


using namespace boost::algorithm;

void split_str()
{
    cerr << "*** string: boost::algorithm::split string test ***\n";

    std::string str_to_split = "hej,med,dig";
    std::vector<std::string> splitted;
    auto finder = boost::is_any_of(",");
    boost::algorithm::split(splitted, str_to_split, finder);
    for (const auto& part: splitted) {
        cerr << "part: '" << part << "'\n";
    }
}

void split_iterator_str()
{
    cerr << "*** string: boost::algorithm::split_iterator string test ***\n";

    const std::string str_to_split = "hej,med,dig";
    auto it_finder = boost::first_finder(",", boost::algorithm::is_equal() );
    using string_split_iterator = boost::algorithm::split_iterator<string::const_iterator>;
    auto split_it  = boost::algorithm::make_split_iterator (str_to_split, it_finder);
    for(; split_it!= string_split_iterator(); ++split_it){
        cerr << "ITERATOR PART: " << *split_it << "\n";
    }
}

void cpaf_split_iterator_str()
{
    cerr << "*** string: cpaf_split_iterator_str string test ***\n";
    using namespace cpaf;

    const std::string str_to_split = "cpaf,hej,med,dig";
    str_const_range range(str_to_split.begin(), str_to_split.end());
//    auto split_it  = mk_str_split_const_iterator (str_to_split, ",");
    auto split_it  = mk_str_split_const_iterator (range, ",");
    for(; split_it!= str_split_const_iterator(); ++split_it){
        cerr << "CPAF ITERATOR PART: " << *split_it << "\n";
    }
}

void split_u16str()
{
    cerr << "*** u16string: boost::algorithm::split u16string test ***\n";

    std::u16string str_to_split = u"hej,med,dig";
    cpaf::unicode::u16_vec splitted;
    auto finder = boost::is_any_of(u",");

    boost::algorithm::split(splitted, str_to_split, finder);
    for (const auto& part: splitted) {
        cerr << "u16 part: '" << part << "'\n";
    }
}

void split_u16str_range()
{
    cerr << "*** u16string: boost::algorithm::split u16string range/pair test ***\n";

    std::u16string str_to_split = u"hej,med,dig";
    cpaf::unicode::u16_const_range_vec splitted;
    auto finder = boost::is_any_of(u",");

    boost::algorithm::split(splitted, str_to_split, finder);
    for (const auto& part: splitted) {
        std::u16string s(part.first, part.second);
        cerr << "u16 range part: '" << s << "'\n";
    }
}

void split_iterator_u16str()
{
    cerr << "*** u16string: boost::algorithm::split_iterator  test ***\n";

    std::u16string str_to_split = u"hej,med,dig";
    auto it_finder = boost::first_finder(",", boost::algorithm::is_equal() );
    using string_split_iterator = boost::algorithm::split_iterator<u16string::iterator>;
    auto split_it  = boost::algorithm::make_split_iterator (str_to_split, it_finder);
    for(; split_it!= string_split_iterator(); ++split_it) {
        //std::u16string s(split_it->begin(), split_it->end());
        std::u16string_view s = cpaf::unicode::mk_u16_view(split_it->begin(), split_it->end());
        cerr << "u16 ITERATOR PART: " << s << "\n";
    }
}

int main()
{
    cerr << "--- u16 boost algorithm playground ---\n";
    split_str();
    split_iterator_str();
    cpaf_split_iterator_str();
    split_u16str();
    split_u16str_range();
    split_iterator_u16str();
    return 0;
}



