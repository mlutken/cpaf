#include <iostream>
#include <vector>
#include <string>
#include <codecvt>
#include <locale>
#include <iomanip>
#include <bitset>
#include <memory>
#include <filesystem>
#include <sparsehash/dense_hash_map>

#include <streams/cpaf_json_streams.h>
#include <filesystem/cpaf_special_dirs.h>
#include <unicode/cpaf_u16string_utils.h>
#include <semantic/classifier/collapse_string.h>
#include <semantic/cpaf_semantic.h>
#include <semantic/primitives/pattern_matcher.h>
#include <semantic/fragments/fragment_matcher.h>
#include <semantic/classifier/fragments_classifier.h>



using json = nlohmann::json;
namespace fs = std::filesystem;
namespace cfs = cpaf::filesystem;
namespace cs = cpaf::streams;
namespace con = cpaf::containers;
namespace cu = cpaf::unicode;

using namespace std;

static const fs::path g_json_data_path = fs::path(__FILE__).parent_path() / "playground.json";

struct my_node;


using my_map_type = std::map<std::u16string, my_node>;
//using cpaf::osops::operator<<;
using namespace cpaf::unicode::osops;
using namespace cpaf::semantic;


using debug_scores_map  = std::unordered_map<std::u16string, std::u16string>;

int main ()
{
    std::vector<std::u16string> types_to_remove = {
        u"sex_property",
        u"colors_property",
        u"item_type",
        u"collapse_name",
        u"brand_name"
    };
    collapse_string::only_remove_if_map only_remove_if;
//      only_remove_if[u"colors_property"] = {{u"blå"}};
      only_remove_if[u"brand_name"] = {{u"scott"}};

    fragments_classifier classifier;
    collapse_string collapser(classifier.fragment_match_ptr());
    classifier.add(u"{¤} liter", u"volume_property", u"liter", 30);
    classifier.add(u"dame", u"sex_property", u"f", 40);
    classifier.add(u"racercykel", u"item_type", u"road_bike", 40);
    classifier.add(u"dame racercykel", u"use_type", u"sport", 50); // note that even though we would match this normally we will not given the types_to_remove
    classifier.add(u"blå", u"colors_property", u"blue", 30);
    classifier.add(u"grøn", u"colors_property", u"green", 30);
    classifier.add(u"sort", u"colors_property", u"black", 30);
    classifier.add(u"62 cm", u"collapse_name", u"y", 1);
    classifier.add(u"scott", u"brand_name", u"Scott", 40);
    classifier.add(u"scott", u"brand_name", u"Scott", 40);

    std::u16string src_string = u"Blå grøn Scott Speedster 2 Apple Dame racercykel 62 cm (ting i parentes)";
    cerr << "src_string                : '" << src_string << "'\n";
    cerr << "remove_matching_attributes: '" << collapser.remove_matching_attributes(src_string, types_to_remove, only_remove_if) << "'\n";
    cerr << "collapse_title            : '" << collapser.collapse_title(src_string, types_to_remove, only_remove_if) << "'\n";

    types_to_remove = std::vector<std::u16string>{u"sex_property"};
    cerr << "Again with new set of attrubutes to remove\n";
    cerr << "src_string                : '" << src_string << "'\n";
    cerr << "remove_matching_attributes: '" << collapser.remove_matching_attributes(src_string, types_to_remove, only_remove_if) << "'\n";
    cerr << "collapse_title            : '" << collapser.collapse_title(src_string, types_to_remove, only_remove_if) << "'\n";

//    collapser.collapse_attributes_set( std::vector<std::u16string>{});
//    src_string = u"Sort Scott Speedster 2 Dame racercykel";
//    cerr << "Hmmm\n";
//    cerr << "src_string                : '" << src_string << "'\n";
//    cerr << "remove_matching_attributes: '" << collapser.remove_matching_attributes(src_string, types_to_remove, only_remove_if) << "'\n";


    return 0;

}

int main_3()
{
    cerr << "--- collapse_name playground ---\n";
    const std::vector<std::u16string> types_to_remove = {
        u"sex_property", u"colors_property", u"item_type"
    };

    const collapse_string::only_remove_if_map only_remove_if;
    fragments_classifier classifier;
    collapse_string collapser(classifier.fragment_match_ptr());
    classifier.add(u"{¤} liter", u"volume_property", u"liter", 30);
    classifier.add(u"dame", u"sex_property", u"f", 40);
    classifier.add(u"racercykel", u"item_type", u"road_bike", 40);
    classifier.add(u"blå", u"colors_property", u"blue", 30);
//    fields.add(u"item_title", u"blå scott speedster 50 dame racercykel");


//    std::u16string src_string = u"Blå Scott Speedster 50 Dame racercykel";
//    const std::u16string src_string = u"Scott Speedster 50";
    std::u16string src_string = u"  Blå  Scott Speedster    50 Dame racercykel ";// Fails!

    std::u16string dst_string;
//    dst_string = collapser.collapse_title(u"Scott Speedster fe-f (tilbud)", types_to_remove);
//    cerr << "src_string: '" << src_string << "'\n";
//    cerr << "dst_string: '" << dst_string << "'\n";

//    src_string = u"Scott Speedster 50-2";
    src_string = u"Scott Speedster last";
//    dst_string = collapser.collapse_title(u"Scott Speedster 50-2 (tilbud)", types_to_remove, only_remove_if);
//    dst_string = collapser.collapse_title(src_string, types_to_remove);
    dst_string = collapser.remove_matching_attributes(src_string, types_to_remove, only_remove_if);
    cerr << "src_string: '" << src_string << "'\n";
    cerr << "dst_string: '" << dst_string << "'\n";

    return 0;
}



int main_1()
{
    cerr << "--- collapse_name playground ---\n";

    fragments_classifier classifier;
    classifier.dbg_mode(true);
//    classifier.add(u"scott {*?} speedster 50", u"master_name", u"speedster 50", 60, u"item_title");
//    classifier.add(u"scott {*?} speedster 50", u"brand_name", u"Scott", 60, u"item_title");
//    classifier.add(u"scott {*?} speedster 50", u"item_type", u"road_bike", 100, u"item_title");
//    classifier.add(u"scott speedster 50", u"master_name", u"speedster 50", 60, u"item_title");
//    classifier.add(u"scott speedster 50", u"brand_name", u"Scott", 60, u"item_title");
    classifier.add(u"scott speedster 50", u"item_type", u"road_bike", 100, u"item_title");
    classifier.add(u"scott", u"brand_name", u"Scott", 70, u"item_title");

    source_fields fields;
    fields.extended_debug_mode(true);
//    fields.add(u"item_title", u"blå scott superfed og sej speedster 50 kvinde racercykel");
//    fields.add(u"item_title", u"blå scott speedster 50 kvinde racercykel");
    fields.add(u"item_title", u"scott speedster 50");
    cerr << "*** classifier.classify() ***\n";
    cerr << "fields.tokens('item_title'): " << dbg_string(fields.tokens(u"item_title")) << "\n";

    const std::vector<std::u16string> restrict_to_types = {u"sex_property", u"colors_property"};

    using record_t = std::unordered_map<std::u16string, std::u16string>;
    record_t rec;
//    classifier.detect_values(rec, fields, parse_mode::longest_full_sentence, restrict_to_types);
    classifier.detect_values(rec, fields, parse_mode::longest_sentence);
//    classifier.classify(fields, sentence_matcher::parse_mode::longest_full_sentence);
    cerr << "--- scores ---\n";
    cerr << fields.scores().extended_debug_string() << "\n";
    cerr << "--- detected values ---\n";
    for (const auto& [key, val]: rec) {
        cerr << key << ": " << val << "\n";
    }

    return 0;
}

int main_std_string_collapse()
{
    cerr << "--- collapse_name playground ---\n";
    const std::vector<std::string> types_to_remove = {
        "sex_property", "colors_property", "item_type"
    };

    const collapse_string::only_remove_if_map only_remove_if;
    fragments_classifier classifier;
    collapse_string collapser(classifier.fragment_match_ptr());
    classifier.add(u"{¤} liter", u"volume_property", u"liter", 30);
    classifier.add(u"dame", u"sex_property", u"f", 40);
    classifier.add(u"racercykel", u"item_type", u"road_bike", 40);
    classifier.add(u"blå", u"colors_property", u"blue", 30);
    classifier.add(u"62 Cm", u"collapse_name", u"y", 30);
//    fields.add(u"item_title", u"blå scott speedster 50 dame racercykel");


//    std::u16string src_string = u"Blå Scott Speedster 50 Dame racercykel";
//    const std::u16string src_string = u"Scott Speedster 50";
    const std::u16string src_string = u"  Blå  Scott Speedster  62 Cm  50 Dame racercykel ";// Fails!

    auto dst_string = collapser.remove_matching_attributes(u"Scott Speedster 50", cu::to_u16string(types_to_remove), only_remove_if);
    cerr << "src_string: '" << src_string << "'\n";
    cerr << "dst_string: '" << dst_string << "'\n";


    return 0;
}
