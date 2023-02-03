#include <iostream>
#include <memory>
#include <gtest/gtest.h>
#include <semantic/classifier/collapse_string.h>
#include <semantic/classifier/fragments_classifier.h>

using namespace testing;
using namespace cpaf::semantic;
using namespace cpaf::unicode::osops;

// https://github.com/google/googletest/blob/master/googletest/docs/faq.md

class CollapseStringUnitTest : public testing::Test
{
public:
    CollapseStringUnitTest()
    {
    }

    ~CollapseStringUnitTest() override = default;

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------

using u8s = std::u16string;

TEST_F(CollapseStringUnitTest, test_collapse_standard)
{
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

    EXPECT_EQ(u"Scott Speedster 50"  ,collapser.remove_matching_attributes(u"Blå Scott Speedster 50 Dame racercykel", types_to_remove, only_remove_if));
    EXPECT_EQ(u"Scott Speedster 50"  ,collapser.remove_matching_attributes(u"  Blå Scott Speedster    50 Dame racercykel ", types_to_remove, only_remove_if));
    EXPECT_EQ(u"Scott Speedster 50"  ,collapser.remove_matching_attributes(u"Blå Scott Speedster    50 Dame racercykel", types_to_remove, only_remove_if));
    EXPECT_EQ(u"Scott Speedster 50"  ,collapser.remove_matching_attributes(u"Scott Speedster 50", types_to_remove, only_remove_if));
    EXPECT_EQ(u"Scott Speedster 50"  ,collapser.remove_matching_attributes(u" Scott Speedster 50 ", types_to_remove, only_remove_if));
    EXPECT_EQ(u"Scott Speedster 50"  ,collapser.remove_matching_attributes(u"  Scott  Speedster  50  ", types_to_remove, only_remove_if));
    EXPECT_EQ(u8s(u"Scott Speedster 50")  ,collapser.remove_matching_attributes(u"Scott blå Speedster 50", types_to_remove, only_remove_if));

//    EXPECT_EQ(u"Scott Speedster 50"  ,collapser.remove_matching_attributes(u"  Blå  Scott Speedster    50 Dame racercykel ", types_to_remove)); // Fails!

//   EXPECT_EQ(u"Scott Speedster 50"  ,collapser.remove_matching_attributes(u"  Blå  Scott  Speedster   50 Dame  ", types_to_remove)); // Fails
}

TEST_F(CollapseStringUnitTest, test_collapse_edge_cases)
{
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

    EXPECT_EQ(u""  ,collapser.remove_matching_attributes(u"", types_to_remove, only_remove_if));
    EXPECT_EQ(u"a"  ,collapser.remove_matching_attributes(u"a", types_to_remove, only_remove_if));
    EXPECT_EQ(u"A"  ,collapser.remove_matching_attributes(u"A", types_to_remove, only_remove_if));
    EXPECT_EQ(u"1"  ,collapser.remove_matching_attributes(u"1", types_to_remove, only_remove_if));
    EXPECT_EQ(u"12"  ,collapser.remove_matching_attributes(u"12", types_to_remove, only_remove_if));
    EXPECT_EQ(u"We did not match anything"  ,collapser.remove_matching_attributes(u"We did not match anything", types_to_remove, only_remove_if));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
