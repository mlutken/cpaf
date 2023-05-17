#include <iostream>
#include <vector>
#include <string>
#include <codecvt>
#include <locale>
#include <iomanip>
#include <bitset>
#include <memory>
#include <thread>
#include <filesystem>
#include <sparsehash/dense_hash_map>

#include <streams/cpaf_json_streams.h>
#include <filesystem/cpaf_special_dirs.h>
#include <unicode/cpaf_u16string_utils.h>
#include <semantic/cpaf_semantic.h>
#include <semantic/primitives/pattern_matcher.h>
#include <semantic/fragments/fragment_matcher.h>
#include <semantic/classifier/fragments_classifier.h>
#include <semantic/readers/json_classification_data_reader.h>
#include <semantic/config/configuration.h>

using json = nlohmann::json;
namespace fs = std::filesystem;
namespace cfs = cpaf::filesystem;
namespace cs = cpaf::streams;
namespace con = cpaf::containers;
namespace cu = cpaf::unicode;

using namespace std;

fs::path g_thehive_semantics_dir = cfs::special_dirs::home() / "code/thehive/phplib/data/text/semantics_data";
fs::path g_cycling_semantics_dir = cfs::special_dirs::home() / "code/thehive/cycling/data/text/semantics_data";
//fs::path g_json_data_path = fs::path(__FILE__).parent_path() / "playground.json";
//fs::path g_bike_type_path = cfs::special_dirs::home() / "code/thehive/cycling/data/text/semantics_data/bike_type.json";
//fs::path g_clothes_type_path = cfs::special_dirs::home() / "code/thehive/cycling/data/text/semantics_data/clothes_type.json";


nlohmann::json g_json_rec = R"(
{
    "item_url":"http://www.wiggle.dk/giro-switchblade-mips-helmet/"
    ,"item_subtype":""
    ,"item_title":"Giro Switchblade MIPS hjelm - blå"
    ,"item_price":"299.95"
    ,"currency_code":"dkk"
    ,"breadcrumbs_text": "hjelme til mænd, blå"
    ,"country_code":"dk"
    ,"language_code":"da"
    ,"description_text":"triggerbug. the ultimate downhill  helmet featuring a removable chinbar combines aggressive styling and superior ventilation."
    ,"_description_text":"the ultimate downhill  helmet featuring a removable chinbar combines aggressive styling and superior ventilation."
}
)"_json;

using namespace cpaf::unicode::osops;
using namespace cpaf::semantic;

using namespace std::chrono_literals;


int main()
{
    cerr << "--- semantic playground ---\n";
    configuration conf;
    conf.rules().add_rule(u"*", 24, 16, u"");
    conf.data_resources().add_resource(g_thehive_semantics_dir, "*.json");
    conf.data_resources().add_resource(g_cycling_semantics_dir, "*.json");
//    conf.data_resources().add_resource(g_bike_type_path);
//    conf.data_resources().add_resource(g_clothes_type_path);

    fragments_classifier fragments_classify__da;
    fragments_classify__da.rules(conf.rules_ptr());
    fragments_classify__da.language_code(u"da");

    fragments_classifier fragments_classify__en;
    fragments_classify__en.rules(conf.rules_ptr());
    fragments_classify__en.language_code(u"en");

    cerr << "Add all fragments from json files...";
    for (const auto& p : conf.data_resources().local_files()) {
//        cerr << "file: " << p << "\n";
        json_classification_data_reader reader(p);
        fragments_classify__da.add_fragments(reader, u"item_type");
//        fragments_classify__en.add_fragments(reader, u"item_type");
    }
    cerr << "Done! Start classifying.\n";


//    {
//        using record_t = std::unordered_map<std::u16string, std::u16string>;
//        using vec_t = std::vector<std::u16string>;
//        vec_t field_names{u"item_title", u"breadcrumbs_text", u"any_field_name"};

//        record_t src_fields_rec;
//        src_fields_rec[u"item_title"]          = u"herrecykler til mænd og kvinder";
//        src_fields_rec[u"breadcrumbs_text"]    = u"herrecykler til mænd og kvinder";
//        src_fields_rec[u"any_field_name"]      = u"herrecykler til mænd og kvinder";

//        source_fields fields;
//        fields.add_fields(src_fields_rec, field_names);

//        record_t rec;
//        fragments_classify__da.detect_values(rec, fields, parse_mode::longest_full_sentence);
//        cerr << fields.dbg_string() << "\n";
//        cerr << "--- detected values ---\n";
//        for (const auto& [key, val]: rec) {
//            cerr << key << ": " << val << "\n";
//        }
//        cerr << "*** Done detecting ***\n\n";
//    }
    {
        using record_t = nlohmann::json;
        using vec_t = std::vector<std::string>;
//        vec_t field_names{"item_title", "breadcrumbs_text", "description_text"};
        vec_t field_names{"item_title", "breadcrumbs_text", "description_text"};

        record_t price_record = g_json_rec;
        source_fields fields;
        fields.extended_debug_mode(true);
        fields.add_fields(price_record, field_names);
//        fields.add("item_title", "herrecykler til mænd og. kvinder");

        fragments_classify__da.detect_values_str(price_record, fields, parse_mode::longest_sentence);
        cerr << fields.dbg_string() << "\n";
        cerr << "--- Price record with detected fields added ---\n";
        cerr << price_record.dump(4) << "\n";
        cerr << "*** Done detecting ***\n";
    }


//    cerr << "Start sleeping....\n";
//    std::this_thread::sleep_for(20s);
//    cerr << "Awake! Exiting!\n";

    return 0;
}


int main_1()
{
    fragment_matcher sentences;

    cerr << "--- semantic playground ---\n";
    token_parser p1(u"   1.4 liter ged");
    p1.parse();
    cerr << "tokens.size(): " << p1.size() << "\n";
    for(const auto& token : p1.tokens())   {
        string is_space = "";
        if (token.attributes().is_separator()) {
            is_space = " [SEPERATOR] ";
        }
        cerr << "Token: " << token.dbg_string()
             << is_space
             << " number_value: " << token.number_value(cu::chars16::dot, cu::chars16::comma)
             << "\n";
    }


    sentences.dbg_mode(false);
    {
//        auto nodes = sentences.add(u"{¤} liter ged hval");
//        cerr << "DEBUG PRINT node: " << nodes[0]->dbg_string() << "\n";
    }
    sentences.dbg_mode(false);
    {
//        auto nodes = sentences.add(u"{¤} liter");
//        cerr << "DEBUG PRINT node: " << nodes[0]->dbg_string() << "\n";
    }
    sentences.dbg_mode(false);
    {
//        auto nodes = sentences.add(u"{¤} liter vand");
//        cerr << "DEBUG PRINT node: " << nodes[0]->dbg_string() << "\n";
    }
    {
//        auto nodes = sentences.add(u"{~} liter vand");
//        cerr << "DEBUG PRINT node: " << nodes[0]->dbg_string() << "\n";
    }
    const std::vector<std::u16string> restrict_to_attributes;


    auto begin = p1.tokens().begin();
    fragment_matcher::candidates_vector candidates = sentences.parse_and_mark_all_tokens(
                begin, p1.tokens().end(), parse_mode::longest_sentence, restrict_to_attributes, 6);

    fragment_matcher::dbg_print_candidates(candidates);

    return 0;
}





int main1()
{
    fragment_matcher sentences;

    sentences.add(u"taske {*} bagagebærer");
    const auto match_str = sentences.match_sentence( u"taske hvad som helst bagagebærer" );
    cerr << "match_str: '" << match_str << "'\n";

    cerr << "--- semantic playground ---\n";
    token_parser p1(u"1.4 liter");
//        p1.string_lookup(sentences.string_lookup_ptr());
//    p1.dbg_mode(true);
    p1.parse();
    cerr << "tokens.size(): " << p1.size() << "\n";
    for(const auto& token : p1.tokens())   {
        string is_space = "";
        if (token.attributes().is_separator()) {
            is_space = " [SEPERATOR] ";
        }
        cerr << "Token: " << token.dbg_string()
             << is_space
             << " number_value: " << token.number_value(cu::chars16::dot, cu::chars16::comma)
             << "\n";
    }

    sentences.dbg_mode(true);
    auto nodes = sentences.add(u"{¤} liter");
    cerr << "DEBUG PRINT node: " << nodes[0]->dbg_string() << "\n";


    auto begin = p1.tokens().begin();
    [[maybe_unused]] skipped_tokens skipped_ranges;

    const std::vector<std::u16string> attributes_collapse;


    auto n = sentences.parse_next_node(skipped_ranges, begin, p1.tokens().end(), parse_mode::longest_sentence, attributes_collapse);
    cerr << "\nFOUND: " <<  n->dbg_string() << "\n";



    return 0;
}



