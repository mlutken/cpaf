#include <gtest/gtest.h>


using namespace testing;

class LocklessFifoUnitTest : public testing::Test
{
public:
    LocklessFifoUnitTest()  = default;

    ~LocklessFifoUnitTest() override = default;

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

TEST_F(LocklessFifoUnitTest, test_test)
{
    EXPECT_EQ(6, 6);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
