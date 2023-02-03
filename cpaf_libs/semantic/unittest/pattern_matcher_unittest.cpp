#include <iostream>
#include <memory>
#include <gtest/gtest.h>
#include <semantic/primitives/pattern_matcher.h>

using namespace testing;
namespace cu = cpaf::unicode;
using namespace cpaf::semantic;

// https://github.com/google/googletest/blob/master/googletest/docs/faq.md

class PatternMatcherUnitTest : public testing::Test
{
public:
    PatternMatcherUnitTest()
    {
    }

    ~PatternMatcherUnitTest() override = default;

    void SetUp() override
    {
        classify_ = std::make_unique<pattern_matcher>();
        classify_->add(u"addict rc ultimate");
        classify_->add(u"addict rc ultimate");
        classify_->add(u"addict rc pro");
        classify_->add(u"addict rc");
        classify_->add(u"addict se disc");

        classify_->add(u"cykel");
        classify_->add(u"cyk#e");
        classify_->add(u"cyk*l");

        // Number matcher
        classify_->add(u"¤");
        classify_->add(u"¤¤");
        classify_->add(u"¤¤¤");
        classify_->add(u"¤¤¤¤");
        classify_->add(u"¤¤¤¤¤");
        classify_->add(u"¤¤¤¤¤¤");
        classify_->add(u"¤¤¤¤¤¤¤");
        classify_->add(u"¤¤¤¤¤¤¤¤");
        classify_->add(u"¤¤¤¤¤¤¤¤¤");

    }

    void TearDown() override
    {
    }
    std::unique_ptr<pattern_matcher> classify_;
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------

TEST_F(PatternMatcherUnitTest, test_lookup3)
{
    classify_->complete_match_mode(true);
    classify_->add(u"racercy*");
    classify_->add(u"racercykel");

////    EXPECT_EQ(u"racercy*"       ,classify_->match_string            ( u"racercy" ));

    // FIXMENM ENABLE AFTER DEBUGGING DONE !!
    EXPECT_EQ(u"racercy*"       ,classify_->match_string            ( u"racercykler" ));
    EXPECT_EQ(u"racercy*"       ,classify_->match_string            ( u"racercyk" ));
    EXPECT_EQ(u"racercykel"     ,classify_->match_string            ( u"racercykel" ));
}


#if 1
TEST_F(PatternMatcherUnitTest,  test_lookup1)
{
    classify_->complete_match_mode(false);
    EXPECT_EQ(u"cykel"      ,classify_->match_string            ( u"cykel"  ));
    EXPECT_EQ(u"cyk*"       ,classify_->match_substring_strict  ( u"cykler" ));
    EXPECT_EQ(u"cyk*"       ,classify_->match_substring_strict  ( u"cyk5er" ));
    EXPECT_EQ(u"cyk#e"      ,classify_->match_string            ( u"cyk2e"  ));
    EXPECT_EQ(u"cyk*l"      ,classify_->match_string            ( u"cyk_l"  ));
    EXPECT_EQ(u""           ,classify_->match_string            ( u"cyk_@"  ));
    EXPECT_EQ(u"cyk*"       ,classify_->match_substring_strict  ( u"cyk_@"  ));
    EXPECT_EQ(u"cyk*l"      ,classify_->match_string            ( u"cyk__l" ));
    EXPECT_EQ(u""           ,classify_->match_string            ( u"cyk__@" ));
    EXPECT_EQ(u"cyk*"       ,classify_->match_substring_strict  ( u"cyk__@" ));
    EXPECT_EQ(u"cy"         ,classify_->match_substring_strict  ( u"cy" ));
    EXPECT_EQ(u""           ,classify_->match_string            ( u"cy" ));
    EXPECT_EQ(u""           ,classify_->match_substring         ( u"cystativ" ));

    // Numbers matching
    EXPECT_EQ(u"¤¤¤¤"       ,classify_->match_string            ( u"1235"       ));
    EXPECT_EQ(u"¤¤¤¤¤¤¤"    ,classify_->match_string            ( u"1235.24"    ));
    EXPECT_EQ(u"¤¤¤¤¤¤¤¤"   ,classify_->match_string            ( u"1,235.23"   ));

    EXPECT_EQ(u"addict rc ultimate" ,classify_->match_string            ( u"addict rc ultimate" ));
    EXPECT_EQ(u"addict rc ultimat"  ,classify_->match_substring_strict  ( u"addict rc ultimat" ));
    EXPECT_EQ(u""                   ,classify_->match_string            ( u"addict rc ultimat" ));
    EXPECT_EQ(u"addict rc pro"      ,classify_->match_string            ( u"addict rc pro" ));
    EXPECT_EQ(u"addict rc pr"       ,classify_->match_substring_strict  ( u"addict rc pr" ));
    EXPECT_EQ(u""                   ,classify_->match_string            ( u"addict rc pr" ));
    EXPECT_EQ(u"addict rc"          ,classify_->match_string            ( u"addict rc" ));
    EXPECT_EQ(u"addict r"           ,classify_->match_substring_strict  ( u"addict r" ));
    EXPECT_EQ(u""                   ,classify_->match_string            ( u"addict r" ));
}


TEST_F(PatternMatcherUnitTest,  test_lookup2)
{
    classify_->complete_match_mode(false);
    classify_->add(u"cyk*");
    classify_->add(u"cyk~l");

    EXPECT_EQ(u"cykel"      ,classify_->match_string            ( u"cykel"  ));
    EXPECT_EQ(u"cyk*"       ,classify_->match_string            ( u"cykler" ));
    EXPECT_EQ(u"cyk*"       ,classify_->match_string            ( u"cyk5er" ));
    EXPECT_EQ(u"cyk#e"      ,classify_->match_string            ( u"cyk2e"  ));
    EXPECT_EQ(u"cyk~l"      ,classify_->match_string            ( u"cyk_l"  ));
    EXPECT_EQ(u"cyk*"       ,classify_->match_string            ( u"cyk_@"  ));
    EXPECT_EQ(u"cyk*l"      ,classify_->match_string            ( u"cyk__l" ));
    EXPECT_EQ(u"cyk*"       ,classify_->match_string            ( u"cyk__@" ));
    EXPECT_EQ(u"cy"         ,classify_->match_substring_strict  ( u"cy" ));
    EXPECT_EQ(u""           ,classify_->match_string            ( u"cy" ));
    EXPECT_EQ(u""           ,classify_->match_substring         ( u"cystativ" ));


    EXPECT_EQ(u"addict rc ultimate" ,classify_->match_string            ( u"addict rc ultimate" ));
    EXPECT_EQ(u"addict rc ultimat"  ,classify_->match_substring_strict  ( u"addict rc ultimat" ));
    EXPECT_EQ(u""                   ,classify_->match_string            ( u"addict rc ultimat" ));
    EXPECT_EQ(u"addict rc pro"      ,classify_->match_string            ( u"addict rc pro" ));
    EXPECT_EQ(u"addict rc pr"       ,classify_->match_substring_strict  ( u"addict rc pr" ));
    EXPECT_EQ(u""                   ,classify_->match_string            ( u"addict rc pr" ));
    EXPECT_EQ(u"addict rc"          ,classify_->match_string            ( u"addict rc" ));
    EXPECT_EQ(u"addict r"           ,classify_->match_substring_strict  ( u"addict r" ));
    EXPECT_EQ(u""                   ,classify_->match_string            ( u"addict r" ));
}



TEST_F(PatternMatcherUnitTest, test_lookup4)
{
    classify_->complete_match_mode(true);
    classify_->add(u"racer");
    classify_->add(u"racercy* med styr");
    classify_->add(u"racercykel med styr");
    classify_->add(u"racercykler");

    EXPECT_EQ(u"racercy* med styr"       ,classify_->match_string            ( u"racercykler med styr" ));
    EXPECT_EQ(u"racercykel med styr"     ,classify_->match_string            ( u"racercykel med styr" ));
}

TEST_F(PatternMatcherUnitTest, test_single_word_lookup)
{
    classify_->complete_match_mode(true);
    classify_->add(u"racer");
    classify_->add(u"racercy*");
    classify_->add(u"racercykler");
    classify_->add(u"####");

    EXPECT_EQ(u"racer"                  ,classify_->match_string            ( u"racer" ));
    EXPECT_EQ(u"racercy*"               ,classify_->match_string            ( u"racercykel" ));
    EXPECT_EQ(u"racercykler"            ,classify_->match_string            ( u"racercykler" ));
    EXPECT_EQ(u"####"                   ,classify_->match_string            ( u"2020" ));
}

#endif

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
