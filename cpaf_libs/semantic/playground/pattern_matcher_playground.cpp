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
#include <semantic/cpaf_semantic.h>
#include <semantic/primitives/pattern_matcher.h>
#include <semantic/fragments/fragment_matcher.h>

using json = nlohmann::json;
namespace fs = std::filesystem;
namespace cfs = cpaf::filesystem;
namespace cs = cpaf::streams;
namespace con = cpaf::containers;
namespace cu = cpaf::unicode;

using namespace std;

fs::path g_json_data_path = fs::path(__FILE__).parent_path() / "playground.json";

using namespace cpaf::unicode::osops;
using namespace cpaf::semantic;

int main1()
{
    cerr << "--- pattern_matcher playground ---\n";
    pattern_matcher classify;
    classify.complete_match_mode(true);
    classify.dbg_mode(false);
    classify.add(u"racer");
    classify.add(u"racercy* med styr");
    classify.add(u"racercykel med styr");
    {
        std::u16string lookup = u"racercykler med styr";
        auto node_ptr = classify.lookup(lookup);
        if (node_ptr) {
            cerr << "\nLOOKUP 1 '" << lookup << "': '" << node_ptr->u16string() << "'\n";
        }
    }
    {
        std::u16string lookup = u"racercykel med styr";
        auto node_ptr = classify.lookup(lookup);
        if (node_ptr) {
            cerr << "\nLOOKUP 2 '" << lookup << "': '" << node_ptr->u16string() << "'\n";
        }
    }
    {
        std::u16string lookup = u"racercykel";
        auto node_ptr = classify.lookup(lookup);
        if (node_ptr) {
            cerr << "\nLOOKUP 3 '" << lookup << "': '" << node_ptr->u16string() << "'\n";
        }
    }
    return 0;
}


int main()
{
    cerr << "--- pattern_matcher playground ---\n";
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt_u16;

    std::u16string rack_bags_component_1 = u"taske ? bagagebærer";
    std::u16string rack_bags_component_2 = u"taske * bagagebærer";
    std::u16string item_title_raw1 = u"Easydo - ED-2602 - Taske til bagagebærer - ( Elcykel ) 29,7 Liter - (Sort)";
    std::u16string item_title1 = u"Easydo ED-2602 Taske til bagagebærer Elcykel 29 Liter Sort";
    std::u16string item_title1_lc = cu::to_lower_copy(item_title1);

    cerr <<  "item_title1    : " << item_title1 << "\n";
    cerr <<  "item_title1_lc : " << item_title1_lc << "\n";
//    parser title1_p(item_title_raw1);
//    title1_p.parse();


    cerr << "sizeof(classifier::node): " << sizeof(pattern_classifier_node) << "\n";

    pattern_matcher classify;
    classify.dbg_mode(false);
//    classify.add(u"easydo");
    classify.add(u"addict rc ultimate");
    classify.add(u"addict rc pro");
    classify.add(u"addict rc");
    classify.add(u"addict se disc");

    classify.add(u"cykel");
    classify.add(u"cyk#e");
    classify.add(u"cyk*");
    classify.add(u"cyk*l");
    classify.add(u"cyk~l");
    classify.add(u"cyke~");
    classify.add(u"¤");
    classify.add(u"¤¤");
    classify.add(u"¤¤¤");
    classify.add(u"¤¤¤¤");
    classify.add(u"¤¤¤¤¤");

    if (auto n = classify.lookup(u"cykel"))     { cerr << "  Found 1 : " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"cykler"))    { cerr << "  Found 2 : " << n->dbg_string() << "\n"; }

    if (auto n = classify.lookup(u"cyk5er"))    { cerr << "  Found 3 : " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"cyk2e"))     { cerr << "  Found 4 : " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"cyk_l"))     { cerr << "  Found 5 : " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"cyk_@"))     { cerr << "  Found 6 : " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"cyk__l"))    { cerr << "  Found 7 : " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"cyk__@"))    { cerr << "  Found 8 : " << n->dbg_string() << "\n"; }

    if (auto n = classify.lookup(u"cy"))        { cerr << "  Found 9 : " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"cystativ"))  { cerr << "  Found 10: " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"cyke@"))     { cerr << "  Found 11 : " << n->dbg_string() << "\n"; }

    if (auto n = classify.lookup(u"addict rc ultimat"))     { cerr << "  Found 30: " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"addict rc ultimate"))    { cerr << "  Found 31: " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"addict rc pro"))         { cerr << "  Found 32: " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"addict rc pr"))          { cerr << "  Found 33: " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"addict rc"))             { cerr << "  Found 34: " << n->dbg_string() << "\n"; }
    if (auto n = classify.lookup(u"addict r"))              { cerr << "  Found 35: " << n->dbg_string() << "\n"; }

    if (auto n = classify.lookup(u"12.34"))                 { cerr << "  Found 40: " << n->dbg_string() << "\n"; }

    return 0;
}



