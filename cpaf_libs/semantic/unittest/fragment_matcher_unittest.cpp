#include <iostream>
#include <memory>
#include <gtest/gtest.h>
#include <semantic/primitives/word_classes_lookup.h>
#include <semantic/fragments/fragment_matcher.h>
#include <cpaf_libs/semantic/fragments/skipped_tokens.h>

using namespace testing;
namespace cu = cpaf::unicode;
using namespace cpaf::semantic;

//  https://github.com/google/googletest/blob/master/googletest/docs/faq.md

class FragmentMatcherUnitTest : public testing::Test
{
public:
    FragmentMatcherUnitTest()
    {
    }

    ~FragmentMatcherUnitTest() override = default;

    void SetUp() override
    {
        wcl_.add(u"{{preposition}}", {u"med", u"for", u"til"});
        wcl_.add(u"{{unit_volume}}", {u"liter", u"ml", u"l"});

        sentences_ = std::make_unique<fragment_matcher>();
        sentences_->word_class_lookup_set(&wcl_);

        sentences_->add(u"wildcard {{*?}} test");
        sentences_->add(u"giro");
        sentences_->add(u"cykel");
        sentences_->add(u"addict rc ultimate");
        sentences_->add(u"addict rc pro");
        sentences_->add(u"addict se disc");
        sentences_->add(u"easydo ed-2602");
        sentences_->add(u"easydo 99ed-2602");
        sentences_->add(u"taske til bagagebærer");
        sentences_->add(u"taske {{~}} bagagebærer");
        sentences_->add(u"taske {{*}} bagagebærer");
        sentences_->add(u"taske {{preposition}} bagagebærer");
        sentences_->add(u"{{¤}} {{unit_volume}}");
        sentences_->add(u"{{preposition}} {{unit_volume}}");
        sentences_->add(u"{{¤}} meter");
        sentences_->add(u"racercy* med styr");
        sentences_->add(u"bmx");
        sentences_->add(u"bmx cyk*");
    }

    void TearDown() override
    {
    }
    word_classes_lookup wcl_;
    std::unique_ptr<fragment_matcher> sentences_;
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------

TEST_F(FragmentMatcherUnitTest, test_parse_skipped_ranges)
{
    fragment_matcher sentences;
    sentences.add(u"giro");
    sentences.add(u"herrecykel");

    sentences.add(u"taske {{~}} herrecykel");
    sentences.add(u"taske {{*}} herrecykel {{*}} for");
    sentences.add(u"gear {{*}} skifte");
    sentences.add(u"sort");
    sentences.add(u"fjernbetjent {{~}} elektronisk");

    std::u16string item_title1_lc = u"giro taske sort til din herrecykel med velour for og gear fjernbetjent til elektronisk skifte";
    token_parser p1(item_title1_lc);
    p1.dbg_mode(false);
    p1.parse();

    skipped_tokens skipped1;
    auto nodes = sentences.parse_all(skipped1, p1.tokens().begin(), p1.tokens().end(), parse_mode::longest_sentence);

    EXPECT_EQ(3, nodes.size());
    EXPECT_EQ(u"giro", nodes[0]->u16string());
    auto s1 = nodes[1]->u16string();
    EXPECT_EQ(u"taske {{*}} herrecykel {{*}} for", nodes[1]->u16string());
    EXPECT_EQ(u"gear {{*}} skifte", nodes[2]->u16string());

    EXPECT_EQ(3, skipped1.ranges().size());
    EXPECT_EQ(u"sort til din", u16_view(skipped1.ranges()[0].std_pair()));
    EXPECT_EQ(u"med velour", u16_view(skipped1.ranges()[1].std_pair()));
    EXPECT_EQ(u"fjernbetjent til elektronisk", u16_view(skipped1.ranges()[2].std_pair()));

    skipped_tokens skipped2;
    const auto skip_range0 = skipped1.ranges()[0];
    nodes = sentences.parse_all(skipped2, skip_range0.first, skip_range0.second, parse_mode::longest_sentence);
    EXPECT_EQ(1, nodes.size());
    EXPECT_EQ(u"sort", nodes[0]->u16string());
    EXPECT_EQ(0, skipped2.ranges().size());

    skipped_tokens skipped3;
    const auto skip_range2 = skipped1.ranges()[2];
    nodes = sentences.parse_all(skipped2, skip_range2.first, skip_range2.second, parse_mode::longest_sentence);
    EXPECT_EQ(1, nodes.size());
    EXPECT_EQ(u"fjernbetjent {{~}} elektronisk", nodes[0]->u16string());
    EXPECT_EQ(0, skipped3.ranges().size());
}



#if 1
TEST_F(FragmentMatcherUnitTest, test_lookup1)
{
    // FIXMENM
    EXPECT_EQ(u"taske {{*}} bagagebærer",sentences_->match_subsentence         ( u"taske hvad som helst bagagebærer GED." ));
    // FIXMENM

    EXPECT_EQ(u"wildcard test"          ,sentences_->match_sentence            ( u"wildcard test" ));
    EXPECT_EQ(u"wildcard {{*}} test"    ,sentences_->match_sentence            ( u"wildcard one test" ));
    EXPECT_EQ(u"wildcard {{*}} test"    ,sentences_->match_sentence            ( u" wildcard one two test " ));

    EXPECT_EQ(u"easydo 99ed-2602"       ,sentences_->match_sentence            ( u"easydo 99ed-2602" ));
    EXPECT_EQ(u"easydo ed-2602"         ,sentences_->match_sentence            ( u"easydo ed-2602" ));

    EXPECT_EQ(u"cykel"                  ,sentences_->match_sentence            ( u"cykel"  ));
    EXPECT_EQ(u"addict rc ultimate"     ,sentences_->match_sentence            ( u"addict rc ultimate" ));
    EXPECT_EQ(u""                       ,sentences_->match_sentence            ( u"addict rc ultimat" ));
    EXPECT_EQ(u"addict rc pro"          ,sentences_->match_sentence            ( u"addict rc pro" ));
    EXPECT_EQ(u""                       ,sentences_->match_sentence            ( u"addict rc pr" ));
    EXPECT_EQ(u"addict rc"              ,sentences_->match_subsentence_strict  ( u"addict rc" ));
    EXPECT_EQ(u""                       ,sentences_->match_sentence            ( u"addict rc" ));
    EXPECT_EQ(u""                       ,sentences_->match_sentence            ( u"easydo ed-2602 more text" ));
    EXPECT_EQ(u"taske til bagagebærer"  ,sentences_->match_sentence            ( u"taske til bagagebærer" ));
    EXPECT_EQ(u"taske {{~}} bagagebærer",sentences_->match_sentence            ( u"taske hvadsomhelst bagagebærer" ));
    EXPECT_EQ(u"taske {{*}} bagagebærer",sentences_->match_sentence            ( u"taske hvad som helst bagagebærer" ));

    EXPECT_EQ(u"taske {{*}} bagagebærer",sentences_->match_sentence            ( u"taske hvad som helst bagagebærer" ));
    EXPECT_EQ(u"taske {{*}} bagagebærer",sentences_->match_subsentence         ( u"taske hvad som helst bagagebærer." ));
    EXPECT_EQ(u"taske {{*}} bagagebærer",sentences_->match_subsentence         ( u"taske hvad som helst bagagebærer. More" ));
    EXPECT_EQ(u"taske {{*}} bagagebærer",sentences_->match_subsentence         ( u"taske , ,hvad, som/ .helst bagagebærer" ));
    EXPECT_EQ(u"taske {{*}} bagagebærer",sentences_->match_subsentence         ( u"taske , ,hvad, som/ .helst/s bagagebærer" ));

    EXPECT_EQ(u"taske {{*}} bagagebærer",sentences_->match_subsentence            ( u"taske , ,hvad, som/ .helst bagagebærer." ));
//    EXPECT_EQ(u"taske {{*}} bagagebærer"  ,sentences_->match_subsentence            ( u"taske, hvad som helst bagagebærer" )); // @todo NOT WORKING

/* FIXMENM TODO: Enable again!
    EXPECT_EQ(u"taske {{*}} bagagebærer"  ,sentences_->match_sentence            ( u"taske! hvad som helst bagagebærer" ));
    EXPECT_EQ(u"taske {{*}} bagagebærer"  ,sentences_->match_sentence            ( u"taske/ hvad som helst bagagebærer" ));
    EXPECT_EQ(u"taske {{*}} bagagebærer"  ,sentences_->match_sentence            ( u"taske.!# hvad som helst bagagebærer" ));
    EXPECT_EQ(u"taske {{*}} bagagebærer"  ,sentences_->match_sentence            ( u"taske< hvad som helst bagagebærer" ));
    EXPECT_EQ(u"taske {{*}} bagagebærer"  ,sentences_->match_sentence            ( u"taske> hvad som helst bagagebærer" ));
*/

//    EXPECT_EQ(u"taske {{*}} bagagebærer"  ,sentences_->match_sentence            ( u"taske/hvad som helst bagagebærer" ));

    // Add a match that ends on a separator
    sentences_->add(u"taske. {{*}} bagagebærer");
    EXPECT_EQ(u"taske. {{*}} bagagebærer"  ,sentences_->match_sentence            ( u"taske. hvad som helst bagagebærer" ));

}

TEST_F(FragmentMatcherUnitTest, test_pattern_lookup)
{

    [[maybe_unused]] skipped_tokens skipped_ranges; // For now, there seems no reason to return these to the caller in this case.
    {
        auto nodes = sentences_->parse_all(skipped_ranges, u"her er bmx cykler til salg");
        EXPECT_EQ(1, nodes.size());
        EXPECT_EQ(u"bmx cyk*", nodes[0]->u16string());
    }
    {
        auto nodes = sentences_->parse_all(skipped_ranges, u"racercykel med styr og det hele");
        EXPECT_EQ(1, nodes.size());
        EXPECT_EQ(u"racercy* med styr", nodes[0]->u16string());
    }
    {
        auto nodes = sentences_->parse_all(skipped_ranges, u"racercykler med styr og det hele");
        EXPECT_EQ(1, nodes.size());
        EXPECT_EQ(u"racercy* med styr", nodes[0]->u16string());
    }

    sentences_->add(u"racercykler med styr");
    {
        auto nodes = sentences_->parse_all(skipped_ranges, u"racercykler med styr og det hele");
        EXPECT_EQ(1, nodes.size());
        EXPECT_EQ(u"racercykler med styr", nodes[0]->u16string());
    }
    {
        auto nodes = sentences_->parse_all(skipped_ranges, u"racercykel med styr og det hele");
        EXPECT_EQ(1, nodes.size());
        EXPECT_EQ(u"racercy* med styr", nodes[0]->u16string());
    }
}



TEST_F(FragmentMatcherUnitTest, test_parse1)
{

    [[maybe_unused]] skipped_tokens skipped_ranges; // For now, there seems no reason to return these to the caller in this case.

    auto nodes = sentences_->parse_all(skipped_ranges, u"taske der er til bagagebærer skrald giro ");
//    auto nodes = sentences_->parse_all(u"taske der er til bagagebærer giro ");
    EXPECT_EQ(2, nodes.size());
    EXPECT_EQ(u"taske {{*}} bagagebærer", nodes[0]->u16string());
    EXPECT_EQ(u"giro", nodes[1]->u16string());

    nodes = sentences_->parse_all(skipped_ranges, u"giro taske der er til bagagebærer");
    EXPECT_EQ(2, nodes.size());
    EXPECT_EQ(u"giro", nodes[0]->u16string());
    EXPECT_EQ(u"taske {{*}} bagagebærer", nodes[1]->u16string());

    nodes = sentences_->parse_all(skipped_ranges, u"giro taske til bagagebærer");
    EXPECT_EQ(2, nodes.size());
    EXPECT_EQ(u"giro", nodes[0]->u16string());
    EXPECT_EQ(u"taske til bagagebærer", nodes[1]->u16string());
}

TEST_F(FragmentMatcherUnitTest, test_word_classes_lookup)
{
    const std::vector<std::u16string> restrict_to_attributes;


    [[maybe_unused]] skipped_tokens skipped_ranges; // For now, there seems no reason to return these to the caller in this case.
    EXPECT_EQ(u"{{preposition}} {{unit_volume}}", sentences_->match_sentence ( u"for liter" ));
    EXPECT_EQ(u"taske {{preposition}} bagagebærer", sentences_->match_sentence ( u"taske med bagagebærer" ));
    EXPECT_EQ(u"{{¤}} {{unit_volume}}", sentences_->match_sentence ( u"1.4 liter" ));
    EXPECT_EQ(u"{{¤}} meter", sentences_->match_sentence ( u"1.4 meter" ));

    auto nodes = sentences_->parse_all(skipped_ranges, u"taske med bagagebærer skrald giro ");
    EXPECT_EQ(2, nodes.size());
    EXPECT_EQ(u"taske {{preposition}} bagagebærer", nodes[0]->u16string());
    EXPECT_EQ(u"giro", nodes[1]->u16string());
}

#else

TEST_F(FragmentMatcherUnitTest, test_parse1_FIXMENM)
{
    auto nodes = sentences_->parse_all(u"giro taske der er til bagagebærer");
    EXPECT_EQ(2, nodes.size());
    EXPECT_EQ(u"giro", nodes[0]->u16string());
    EXPECT_EQ(u"taske {{*}} bagagebærer", nodes[1]->u16string());


//    nodes = sentences_->parse_all(u"taske der er til bagagebærer skrald giro ");
//    EXPECT_EQ(2, nodes.size());
//    EXPECT_EQ(u"taske {{*}} bagagebærer", nodes[0]->u16string());
//    EXPECT_EQ(u"giro", nodes[1]->u16string());

//    nodes = sentences_->parse_all(u"giro taske til bagagebærer");
//    EXPECT_EQ(2, nodes.size());
//    EXPECT_EQ(u"giro", nodes[0]->u16string());
//    EXPECT_EQ(u"taske til bagagebærer", nodes[1]->u16string());
}

#endif

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
