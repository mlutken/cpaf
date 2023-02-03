#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <boost/filesystem/path.hpp>
#include <boost/algorithm/string.hpp>
#include <streams/cpaf_json_streams.h>
#include <filesystem/cpaf_special_dirs.h>

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


fs::path g_json_data_path = fs::path(__FILE__).parent_path() / "playground.json";
//fs::path g_json_data_path = fs::path(__FILE__).parent_path() / "clothes_type.json";

using namespace cpaf::unicode::osops;
using namespace cpaf::semantic;

int main()
{
    cerr << "--- fragments_classifier playground ---\n";
    json_classification_data_reader reader(g_json_data_path);

    cerr << "default weights:\n";
    cerr << reader.dbg_default_weights() << "\n";

    cerr << "------ default weights------- \n";

    fragments_classifier fragments_classify;
    classification_rules rules;

    rules.add_rule(u"*", 10, 20, u"");
    rules.add_rule(u"item_type", 11, 21, u"");
    fragments_classify.rules(&rules);

    fragments_classify.language_code(u"da");
//    classifier.dbg_mode(true);
//    classifier.add(u"herrecy*", u"item_type", u"city_bike", 20, u"item_title");
//    classifier.add(u"herrecy*", u"sex", u"m", 20, u"item_title");
//    classifier.add(u"herrecy*", u"sex", u"m", 17, u"breadcrumbs_text");
//    classifier.add(u"herrecykel", u"item_type", u"city_bike", 25 ,u"item_title");
//    classifier.add(u"herrecykel", u"sex", u"m", 25, u"item_title");
//    classifier.add(u"mænd", u"sex", u"m", 30, u"item_title");
//    classifier.add(u"kvinder", u"sex", u"f", 30, u"item_title");

    fragments_classify.add_fragments(reader, u"item_type");

    source_fields fields;
    fields.add(u"item_title", u"herrecykler til mænd og kvinder");
    fields.add(u"breadcrumbs_text", u"herrecykler til mænd og kvinder");
    fields.add(u"any_field_name", u"herrecykler til mænd og kvinder");
    using record_t = std::unordered_map<std::u16string, std::u16string>;
    record_t rec;
    fragments_classify.detect_values(rec, fields, parse_mode::longest_sentence);
    cerr << fields.dbg_string() << "\n";
    cerr << "--- detected values ---\n";
    for (const auto& [key, val]: rec) {
        cerr << key << ": " << val << "\n";
    }
    return 0;
}



