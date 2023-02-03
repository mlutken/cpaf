#include <gtest/gtest.h>

#include <iostream>
#include <unicode/cpaf_u8string_utils.h>

using namespace testing;
using namespace std;
namespace cu = cpaf::unicode;


class U8StringUtilsUnitTest : public testing::Test
{
public:
    U8StringUtilsUnitTest()  = default;

    ~U8StringUtilsUnitTest() override = default;

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

TEST_F(U8StringUtilsUnitTest, parse_substring_between__general_test1)
{
    const string search_in = "find the string between here and the one here in the end";

    {
        auto from = search_in.begin();
        auto subrange = cu::parse_substring_between(from, search_in.end(), "between here", "here");
        EXPECT_EQ(" and the one ", cu::to_string(subrange));
        EXPECT_EQ(" in the end", string(from, search_in.end()));
    }
    {
        auto from = search_in.begin();
        auto subrange = cu::parse_substring_between(from, search_in.end(), "begin marker not found", "here");
        EXPECT_EQ("", cu::to_string(subrange));
        EXPECT_EQ(search_in, string(from, search_in.end()));
    }
    {
        auto from = search_in.begin();
        auto subrange = cu::parse_substring_between(from, search_in.end(), "between here", "end marker not found");
        EXPECT_EQ("", cu::to_string(subrange));
        EXPECT_EQ(search_in, string(from, search_in.end()));
    }
    {
        auto from = search_in.begin();
        auto subrange = cu::parse_substring_between(from, search_in.end(), "begin marker not found", "end marker not found");
        EXPECT_EQ("", cu::to_string(subrange));
        EXPECT_EQ(search_in, string(from, search_in.end()));
    }
}

TEST_F(U8StringUtilsUnitTest, parse_substring_between__general_test2)
{
    const string search_in = "find the string between here and the one here in the end";

    {
        auto from = search_in.begin();
        auto subrange = cu::parse_substring_between(from, search_in.end(), "", "here in");
        EXPECT_EQ("find the string between here and the one ", cu::to_string(subrange));
        EXPECT_EQ(" the end", string(from, search_in.end()));
    }

    {
        auto from = search_in.begin();
        auto subrange = cu::parse_substring_between(from, search_in.end(), "between here", "");
        EXPECT_EQ(" and the one here in the end", cu::to_string(subrange));
        EXPECT_EQ("", string(from, search_in.end()));
    }

    {
        auto from = search_in.begin();
        auto subrange = cu::parse_substring_between(from, search_in.end(), "", "");
        EXPECT_EQ("", cu::to_string(subrange));
        EXPECT_EQ(search_in, string(from, search_in.end()));
    }
}

TEST_F(U8StringUtilsUnitTest, substring_between__general_test1)
{
    const string search_in = "find the string between here and the one here in the end";
    {
        auto substr = cu::substring_between(search_in, "between here", "here");
        EXPECT_EQ("and the one", substr);
    }
    {
        auto substr = cu::substring_between(search_in, "between here", "here", cu::do_trim::yes);
        EXPECT_EQ("and the one", substr);
    }
    {
        auto substr = cu::substring_between(search_in, "between here", "here", cu::do_trim::no);
        EXPECT_EQ(" and the one ", substr);
    }
    {
        auto substr = cu::substring_between(search_in, "begin marker not found", "here");
        EXPECT_EQ("", substr);
    }

    {
        auto substr = cu::substring_between(search_in, "between here", "end marker not found");
        EXPECT_EQ("", substr);
    }
    {
        auto substr = cu::substring_between(search_in, "begin marker not found", "end marker not found");
        EXPECT_EQ("", substr);
    }
}


TEST_F(U8StringUtilsUnitTest, substring_between__general_test2)
{
    const string search_in = "find the string between here and the one here in the end";

    {
        auto substr = cu::substring_between(search_in, "", "here in");
        EXPECT_EQ("find the string between here and the one", substr);
    }
    {
        auto substr = cu::substring_between(search_in, "between here", "");
        EXPECT_EQ("and the one here in the end", substr);
    }
    {
        auto substr = cu::substring_between(search_in, "", "");
        EXPECT_EQ("", substr);
    }
}

TEST_F(U8StringUtilsUnitTest, substring_between__special_chars)
{
    const string search_in1 = R"("_scroll_id" : "DnF1ZXJ5VGhlbkZldGNoBQAAAAAAIfi5FjE5eWd", )";
    {
        auto substr = cu::substring_between(search_in1, R"("_scroll_id" : ")", R"(")");
        EXPECT_EQ("DnF1ZXJ5VGhlbkZldGNoBQAAAAAAIfi5FjE5eWd", substr);
    }
    {
        auto substr = cu::substring_between(search_in1, R"("_scroll_id":")", R"(")");
        EXPECT_EQ("", substr);
    }

    const string search_in2 = R"("_scroll_id":"DnF1ZXJ5VGhlbkZldGNoBQAAAAAAIfi5FjE5eWd", )";
    {
        auto substr = cu::substring_between(search_in2, R"("_scroll_id":")", R"(")");
        EXPECT_EQ("DnF1ZXJ5VGhlbkZldGNoBQAAAAAAIfi5FjE5eWd", substr);
    }
    {
        auto substr = cu::substring_between(search_in2, R"("_scroll_id" : ")", R"(")");
        EXPECT_EQ("", substr);
    }
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
