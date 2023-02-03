#include <iostream>
#include <memory>
#include <gtest/gtest.h>

using namespace testing;

// https://github.com/google/googletest/blob/master/googletest/docs/faq.md

class SemanticUnitTest : public testing::Test
{
public:
    SemanticUnitTest()
    {
    }

    ~SemanticUnitTest() override = default;

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



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
