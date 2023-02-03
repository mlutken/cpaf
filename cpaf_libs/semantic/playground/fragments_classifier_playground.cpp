#include <iostream>
#include <vector>
#include <string>
#include <codecvt>
#include <locale>
#include <iomanip>
#include <bitset>
#include <memory>
#include <boost/filesystem/path.hpp>
#include <sparsehash/dense_hash_map>

#include <streams/cpaf_json_streams.h>
#include <filesystem/cpaf_special_dirs.h>
#include <unicode/cpaf_u16string_utils.h>
#include <semantic/cpaf_semantic.h>
#include <semantic/primitives/pattern_matcher.h>
#include <semantic/fragments/fragment_matcher.h>
#include <semantic/classifier/fragments_classifier.h>
#include <semantic/primitives/word_classes_lookup.h>


using json = nlohmann::json;
namespace fs = boost::filesystem;
namespace cfs = cpaf::filesystem;
namespace cs = cpaf::streams;
namespace con = cpaf::containers;
namespace cu = cpaf::unicode;

using namespace std;

using bitset_128 = std::bitset<128>;
using bitset_512 = std::bitset<512>;
using bitset_4k = std::bitset<0xFFF>;

//rack_bags_component

static const fs::path g_json_data_path = fs::path(__FILE__).parent_path() / "playground.json";

struct my_node;


using my_map_type = std::map<std::u16string, my_node>;
//using cpaf::osops::operator<<;
using namespace cpaf::unicode::osops;
using namespace cpaf::semantic;


using debug_scores_map  = std::unordered_map<std::u16string, std::u16string>;

int play4()
{
    cerr << "--- fragments_classifier playground ---\n";

    fragments_classifier classifier;
//    classifier.dbg_mode(true);

    classifier.add(u"trehjulet cykel", u"item_type", u"tri_bike", -100);
    classifier.add(u"trehjulet cykel", u"item_type", u"learner_bike", 60);
    classifier.add(u"tricykel", u"item_type", u"tri_bike", 100);

    source_fields fields;
    fields.extended_debug_mode(true);
    fields.add(u"item_title", u"winther trehjulet cykel tricykel");
//    fields.add(u"description_text", u"winther trehjulet cykel - mørkeblå");
    cerr << "*** classifier.classify() ***\n";
    cerr << "fields.tokens('item_title'): " << dbg_string(fields.tokens(u"item_title")) << "\n";


    using record_t = std::unordered_map<std::u16string, std::u16string>;
    record_t rec;
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



int play3()
{
    cerr << "--- fragments_classifier playground ---\n";

    fragments_classifier classifier;
//    classifier.dbg_mode(true);
    classifier.add(u"racercy*", u"item_type", u"road_bike", 20);
    classifier.add(u"racercykel", u"item_type", u"road_bike", 25);
    classifier.add(u"citybike", u"item_type", u"city_bike", 15);

    source_fields fields;
    fields.extended_debug_mode(false);
//    fields.add(u"item_title", u"racercykel med styr");
    fields.add(u"item_title", u"racercykler med styr");
    fields.add(u"description_text", u"her er en racercykel til sjov og ikke som citybike");
    cerr << "*** classifier.classify() ***\n";
    cerr << "fields.tokens('item_title'): " << dbg_string(fields.tokens(u"item_title")) << "\n";

//    auto node = classifier.word_pattern_lookup().lookup(u"racercykler");
//    if (node) cerr << "token_match: '" << node->u16string() << "', complete: '" <<  node->is_complete_word() <<  "'\n";

    using record_t = std::unordered_map<std::u16string, std::u16string>;
    record_t rec;
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


int play2()
{
    cerr << "--- fragments_classifier playground ---\n";

    fragments_classifier classifier;
//    classifier.dbg_mode(true);
    classifier.add(u"giro", u"brand_name", u"giro", 20);
    classifier.add(u"bmx cykel", u"item_type", u"bmx_bike", 20);
    classifier.add(u"dame", u"item_type", u"city_bike", 5);
    classifier.add(u"dame", u"sex", u"f", 20);
    classifier.add(u"sort", u"color", u"black", 30);
    classifier.add(u"taske til bagagebærer");
    classifier.add(u"taske til bagagebærer", u"my_type", u"my_bags_component", 40);
//    classifier.add(u"taske {{~}} bagagebærer", u"my_type", u"my_bags_component", 30);
    classifier.add(u"taske {{*}} bagagebærer", u"my_type", u"my_bags_component", 20);
    classifier.add(u"easydo ed-2602");
    classifier.add(u"cykel");
    classifier.add(u"herrecykel");

    source_fields fields;
    fields.extended_debug_mode(true);
    fields.add(u"item_title", u"giro bmx cykel dame taske der er sort bagagebærer");
    cerr << "*** classifier.classify() ***\n";

    using record_t = std::unordered_map<std::u16string, std::u16string>;
    record_t rec;
    classifier.detect_values(rec, fields, parse_mode::longest_sentence_multi_pass);
//    classifier.classify(fields, sentence_matcher::parse_mode::longest_full_sentence);
    cerr << "--- scores ---\n";
//    cerr << fields.scores().debug_string() << "\n";
    cerr << fields.scores().extended_debug_string() << "\n";
    cerr << "--- detected values ---\n";
    for (const auto& [key, val]: rec) {
        cerr << key << ": " << val << "\n";
    }

    return 0;
}

int double_detect_problem()
{
    cerr << "--- double_detect_problem playground ---\n";

    fragments_classifier classifier;
    classifier.add(u"oxc", u"brand_name", u"oxc", 20);
    classifier.add(u"halsedisse", u"item_type", u"headwear_clothes", 20);
    classifier.add(u"fleece", u"materials_property", u"wool", 25);
    classifier.add(u"polyester", u"materials_property", u"polyester", 25);
    classifier.add(u"med {{~}} motor", u"item_type", u"e_bike", 30);
//    classifier.add(u"med {{~}} somethingunique", u"item_type", u"e_bike", 30);
    classifier.add(u"med", u"collapse_name", u"y", 30);

    source_fields fields;
    fields.extended_debug_mode(true);
    fields.add(u"item_title", u"ocx halsedisse med fleece somethingunique");
    cerr << "*** classifier.classify() ***\n";

    using record_t = std::unordered_map<std::u16string, std::u16string>;
    record_t rec;
    classifier.detect_values(rec, fields, parse_mode::longest_sentence_multi_pass);
//    classifier.classify(fields, sentence_matcher::parse_mode::longest_full_sentence);
    cerr << "--- scores ---\n";
//    cerr << fields.scores().debug_string() << "\n";
    cerr << fields.scores().extended_debug_string() << "\n";
    cerr << "--- detected values ---\n";
    for (const auto& [key, val]: rec) {
        cerr << key << ": " << val << "\n";
    }

    return 0;
}

int main()
{
    double_detect_problem();
    return 0;
}


//----------
//Problem med farver:
//----------
//id: df62837e588cfc2332e35bfec172e728105965d8
//item_url: https://www.bikesport.dk/cykeltoj/cykelhjelme/bell-hjelm-sixer-mips-lys-bla-sort-cykelhjelm.html
//classification_text: BELL, HJELM, SIXER, MIPS -, LYS, BLÅ/SORT, Cykelhjelm Voksenhjelme
int main5()
{

    cerr << "--- fragments_classifier playground ---\n";

    fragments_classifier classifier;
//    classifier.dbg_mode(true);
    classifier.add(u"en to tre", u"junk_property", u"junk1", 25);
    classifier.add(u"sort", u"color_property", u"black", 25);
    classifier.add(u"blå", u"color_property", u"blue", 30);

    source_fields fields;
    fields.extended_debug_mode(true);
    fields.add(u"item_title", u"BELL, HJELM, SIXER, MIPS -, LYS, BLÅ/SORT , Cykelhjelm Voksenhjelme");
    cerr << "*** classifier.classify() ***\n";

    using record_t = std::unordered_map<std::u16string, std::u16string>;
    record_t rec;
    classifier.detect_values(rec, fields, parse_mode::longest_sentence);
//    classifier.classify(fields, sentence_matcher::parse_mode::longest_full_sentence);
    cerr << "--- scores ---\n";
//    cerr << fields.scores().debug_string() << "\n";
    cerr << fields.scores().extended_debug_string() << "\n";
    cerr << "--- detected values ---\n";
    for (const auto& [key, val]: rec) {
        cerr << key << ": " << val << "\n";
    }

    return 0;
}

int main6()
{

    cerr << "--- fragments_classifier playground ---\n";

//    word_classes_lookup wcl;
    fragments_classifier classifier;
//    classifier.word_class_lookup_set(&wcl);
    classifier.dbg_mode(false);
    classifier.add(u"briller", u"item_type", u"glasses_clothes", 25);
    classifier.add(u"briller", u"item_type", u"frames_component", -200);

    source_fields fields;
    fields.extended_debug_mode(true);
    fields.add(u"item_title", u"briller til cykel");
    cerr << "*** classifier.classify() ***\n";

    using record_t = std::unordered_map<std::u16string, std::u16string>;
    record_t rec;
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
