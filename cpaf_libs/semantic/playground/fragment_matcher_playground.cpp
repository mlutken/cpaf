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
#include <unicode/cpaf_unicode_chars.hpp>
#include <semantic/cpaf_semantic.h>
#include <semantic/primitives/pattern_matcher.h>
#include <semantic/fragments/fragment_matcher.h>
#include <semantic/primitives/word_classes_lookup.h>

using json = nlohmann::json;
namespace fs = std::filesystem;
namespace cfs = cpaf::filesystem;
namespace cs = cpaf::streams;
namespace con = cpaf::containers;
namespace cu = cpaf::unicode;

using namespace std;

using bitset_128 = std::bitset<128>;
using bitset_512 = std::bitset<512>;
using bitset_4k = std::bitset<0xFFF>;

//rack_bags_component

fs::path g_json_data_path = fs::path(__FILE__).parent_path() / "playground.json";

struct my_node;


using my_map_type = std::map<std::u16string, my_node>;
//using cpaf::osops::operator<<;
using namespace cpaf::unicode::osops;
using namespace cpaf::semantic;


int main()
{
    cerr << "--- fragment_matcher playground ---\n";

    std::u16string item_title1_lc = u"giro taske sort til din herrecykel med velour for og gear til elektronisk skifte";

    fragment_matcher sentences;
//    sentences.dbg_mode(true);
    sentences.add(u"giro");
    sentences.add(u"herrecykel");
//    sentences.add(u"taske sort til herrecykel");

    sentences.add(u"taske {{~}} herrecykel");
    sentences.add(u"taske {{*}} herrecykel {{*}} for");
    //    sentences.add(u"gear til elektronisk skifte");
    sentences.add(u"gear {{*}} skifte");

    //    sentences.add(u"taske {{*}} for");


    token_parser p1(item_title1_lc);
    p1.dbg_mode(false);
    p1.parse();
    cerr << "Tokens: \"" << token::debug_str(p1.tokens()) << "\"\n";

    skipped_tokens skipped_ranges;

    cerr << "!!!!!!!!!!!!!!!START\n";
    const std::vector<std::u16string> restrict_to_attributes;


    auto nodes = sentences.parse_all(skipped_ranges, p1.tokens().begin(), p1.tokens().end(), parse_mode::longest_sentence, restrict_to_attributes);
    for (auto node : nodes) {
        cerr << "Parsed node: " << node->dbg_string() << "\n";
    }
    skipped_ranges.debug_print(); // FIXMENM

    cerr << "!!!!!!!!!!!!!!!END\n";

//    auto begin = p1.tokens().begin();
//    fragment_matcher::candidates_vector candidates = sentences.parse_and_mark_all_tokens(begin, p1.tokens().end(), parse_mode::longest_full_sentence, 6);

//    cerr << "--\n";
//    fragment_matcher::dbg_print_candidates(candidates);

//    token::vector::const_iterator& begin,
//    const token::vector::const_iterator& end,
//    parse_mode pm,
//    size_t size_hint) const

//    sentences.parse_all()
//    const auto end = p1.tokens().end();
//    auto begin1 = p1.tokens().begin();
//    while (begin1 < end) {
//        const auto it_before = begin1;
//        auto n = sentences.parse_next_node(begin1, end, parse_mode::longest_full_sentence);
//        if (n) {
//            const int dist = begin1 - it_before;
//            cerr << "[" << dist <<  "]  Parsed 1 : " << n->dbg_string() << "\n";
//        }
//        else {
//            ++begin1;
//        }
//    }



    return 0;
}





int main_5()
{
    cerr << "--- fragment_matcher playground ---\n";
    word_classes_lookup wcl;
    wcl.add(u"{{preposition}}", {u"med", u"for", u"til"});
    wcl.add(u"{{unit_volume}}", {u"liter", u"ml", u"l"});

    fragment_matcher fragments;
    fragments.word_class_lookup_set(&wcl);

    fragments.add(u"{{preposition}} {{unit_volume}}");
    fragments.add(u"{{¤}} {{unit_volume}}");
    fragments.add(u"http://schema.org/instock");
    fragments.add(u"{{¤}} kroner");

    auto m1 = fragments.match_sentence ( u"for liter");
    auto m2 = fragments.match_sentence ( u"1.4 liter");
    auto m3 = fragments.match_subsentence ( u"http://schema.org/instock");
    auto m4 = fragments.match_sentence ( u"1.4 kroner");

    cerr << "m1: '" << m1 << "'\n";
    cerr << "m2: '" << m2 << "'\n";
    cerr << "m3: '" << m3 << "'\n";
    cerr << "m4: '" << m4 << "'\n";

    return 0;
}



int main_3()
{
    cerr << "--- fragment_matcher playground ---\n";

    auto sentences_ = std::make_unique<fragment_matcher>();
//    sentences_->word_class_lookup_set(&wcl_);

//    sentences_->add(u"easydo ed-2602");
//    sentences_->add(u"easydo 99ed-2602");
//    sentences_->add(u"one two three");
    sentences_->add(u"wildcard {*?} test");

//    auto m1 = sentences_->match_sentence ( u"easydo 99ed-2602" );
//    auto m1 = sentences_->match_sentence ( u"easydo 99ed-2602" );
//    auto m1 = sentences_->match_sentence ( u"easydo sd-23" );
//    auto m1 = sentences_->match_sentence ( u"one two three" );
    auto m1 = sentences_->match_sentence ( u"wildcard!. sd as test" );
    cerr << "\nm1: '" << m1 << "'\n";
//    EXPECT_EQ(u"easydo 99ed-2602"       ,sentences_->match_sentence            ( u"easydo 99ed-2602" ));
//    EXPECT_EQ(u"easydo ed-2602"         ,sentences_->match_sentence            ( u"easydo ed-2602" ));

    return 0;
}

int main_4()
{
    cerr << "--- fragment_matcher playground ---\n";

    pattern_matcher wpl;


    fragment_matcher classifier;
//    classifier.word_pattern_lookup_set(&wpl);
//    classifier.dbg_mode(true);
    classifier.add(u"racercy* med styr");


    const std::u16string str1 = u"racercykel med styr og det hele";
//    const std::u16string str1 = u"racercy* med styr og det hele";
    token_parser p(str1);
    p.parse();
    cerr << "tokens: " << dbg_string(p.tokens()) << "\n";
    auto tk_begin = p.tokens().begin();

    const std::vector<std::u16string> restrict_to_attributes;


    [[maybe_unused]] skipped_tokens skipped_ranges;
    const fragment_node* sen_node1 = classifier.parse_next_node(skipped_ranges, tk_begin, p.tokens().end(), parse_mode::longest_sentence, restrict_to_attributes);
    if (sen_node1) {
        cerr << "node1: '" << sen_node1->u16string() << "'\n";
    }

    return 0;
}


int main_1()
{
    cerr << "--- fragment_matcher playground ---\n";
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt_u16;

    std::u16string rack_bags_component_1 = u"taske ? bagagebærer";
    std::u16string rack_bags_component_2 = u"taske * bagagebærer";
    std::u16string item_title_raw1 = u"Easydo - ED-2602 - Taske til bagagebærer - ( Elcykel ) 29,7 Liter - (Sort)";
    std::u16string item_title1 = u"Easydo ED-2602 Taske til bagagebærer Elcykel 29 Liter Sort";
    std::u16string item_title1_lc = cu::to_lower_copy(item_title1);

    cerr <<  "item_title1    : " << item_title1 << "\n";
    cerr <<  "item_title1_lc : " << item_title1_lc << "\n";
    token_parser title1_p(item_title_raw1);
//    title1_p.dbg_mode(true);
//    title1_p.parse();


    cerr << "sizeof(classifier::node): " << sizeof(pattern_classifier_node) << "\n\n";

    word_classes_lookup wcl;
    wcl.add(u"{preposition}", {u"med", u"for", u"til"});
    wcl.add(u"{unit_volume}", {u"liter", u"ml", u"l"});

    fragment_matcher sentences;
    sentences.word_class_lookup_set(&wcl);


//    sentences.dbg_mode(true);
//    sentences.root()->append(u"taske")->append(u"til")->append_last(u"bagagebærer");
//    sentences.root()->append(u"taske")->append(u"{~}")->append_last(u"bagagebærer");

    sentences.add(u"giro");
    sentences.add(u"taske til bagagebærer");
    sentences.add(u"taske {preposition} bagagebærer");
    sentences.add(u"taske {~} bagagebærer");
    sentences.add(u"taske {*} bagagebærer");
    sentences.add(u"taske {¤} bagagebærer");
    sentences.add(u"{¤} {unit_volume}");
    sentences.add(u"easydo ed-2602");
    sentences.add(u"cykel");
    sentences.add(u"bmx cykel");
    sentences.add(u"herrecykel");


    {
//        token_parser p1(u"giro bmx cykel");
        cerr << "--- Parsing token with numbers ---\n";
//        token_parser p1(u"taske 12.4 bagagebærer");
//        token_parser p1(u"taske med bagagebærer");
        token_parser p1(u"1.4 liter");
//        p1.string_lookup(sentences.string_lookup_ptr());
        p1.dbg_mode(true);
        p1.parse();

        const std::vector<std::u16string> restrict_to_attributes;



        auto begin = p1.tokens().begin();
        [[maybe_unused]] skipped_tokens skipped_ranges;
        while (auto n = sentences.parse_next_node(skipped_ranges, begin, p1.tokens().end(), parse_mode::longest_sentence, restrict_to_attributes))   {
            cerr << "  Parsed 1 : " << n->dbg_string() << "\n";
        }
        for(const auto& token : p1.tokens())   {
            cerr << "Token: " << token.dbg_string()
//                 << " number_value: " << token.number_value(cu::chars16::comma, cu::chars16::dot) << "\n";
                << " number_value: " << token.number_value(cu::chars16::dot, cu::chars16::comma) << "\n";
        }


    }


//    parser p1(u"taske til bagagebærer ged");
//    p1.dbg_mode(true);
//    p1.parse();

//    auto begin = p1.tokens().begin();
//    const auto end = p1.tokens().end();
//    if (auto n = sentences.parse_next(begin, end, sentence_matcher::parse_mode::longest_full_sentence))   {
//        cerr << "  Parsed 1 : " << n->dbg_string() << "\n";
//    }


//    parser p2(u"easydo ed-2602");
//    p2.dbg_mode(true);
//    p2.parse();

//    if (auto n = sentences.lookup_sentence(u"taske 12.4 bagagebærer"))   { cerr << "  Found 0 : " << n->dbg_string() << "\n"; }
//    if (auto n = sentences.lookup_sentence(u"taske til bagagebærer"))   { cerr << "  Found 1 : " << n->dbg_string() << "\n"; }
//    if (auto n = sentences.lookup_sentence(u"taske hvadsomhelst bagagebærer"))   { cerr << "  Found 2 : " << n->dbg_string() << "\n"; }
//    if (auto n = sentences.lookup_sentence(u"taske hvad som helst bagagebærer"))   { cerr << "  Found 3 : " << n->dbg_string() << "\n"; }
//    if (auto n = sentences.lookup_sentence(u"99easydo 12ed-2602"))          { cerr << "  Found 10 : " << n->dbg_string() << "\n"; }
//    if (auto n = sentences.lookup_sentence(u"cykel"))                   { cerr << "  Found 11 : " << n->dbg_string() << "\n"; }

//    cerr << "classify_->match_sentence(\"cykel\"): '" << sentences.match_sentence(u"cykel") << "'\n";

//    cerr << "'taske til bagagebærer': " << sentences.match_sentence(u"taske til bagagebærer") << "\n";
//    cerr << "'taske hvadsomhelst bagagebærer': " << sentences.match_sentence(u"taske hvadsomhelst bagagebærer") << "\n";
//    cerr << "'taske hvad som helst bagagebærer': " << sentences.match_sentence(u"taske hvad som helst bagagebærer") << "\n";

//    {
//        cerr << "Find all in: 'giro taske der er til bagagebærer'\n";
//        auto nodes = sentences.parse_all(u"giro taske der er til bagagebærer");
//        dbg_print(nodes);
//    }
//    {
//        cerr << "Find all in: 'taske der er til bagagebærer giro s'\n";
//        auto nodes = sentences.parse_all(u"taske der er til bagagebærer giro ");
//        dbg_print(nodes);
//    }

//    "bmx cykel",
//    "bmx cyklen",
//    "bmx cykler",
//    "bmx cyklerne",
//    "bmx-cykel",
//    "bmx-cyklen",
//    "bmx-cykler",
//    "bmx-cyklerne"


    return 0;
}



