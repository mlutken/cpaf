#include <gtest/gtest.h>


using namespace testing;

class DynamicArrayUnitTest : public testing::Test
{
public:
    DynamicArrayUnitTest()  = default;

    ~DynamicArrayUnitTest() override = default;

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

TEST_F(DynamicArrayUnitTest, test_test)
{
    EXPECT_EQ(6, 6);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
