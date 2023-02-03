#include <iostream>
#include <gtest/gtest.h>
#include <boost/filesystem/path.hpp>
#include <cpaf_libs/economic/cpaf_exchange_rates.h>

namespace fs = boost::filesystem;
const fs::path exchange_rates_path = fs::path(__FILE__).parent_path() / "exchange_rates.json";

using namespace testing;
using namespace cpaf;
using namespace std;

class ExchangeRatesUnitTest : public testing::Test
{
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------

TEST_F(ExchangeRatesUnitTest, test_test)
{
    exchange_rates er;
    er.set_exchange_rates(exchange_rates_path);

//    auto val1 = er.to_eur("dkk", 1);
//    cerr << "val1: " << val1 << "\n";

//    auto val2 = er.from_eur("dkk", 1);
//    cerr << "val2: " << val2 << "\n";

//    auto val3 = er.convert("dkk", "nok", 1);
//    cerr << "val3: " << val3 << "\n";

//    auto val4 = er.convert("dkk", "nok", 10);
//    cerr << "val4: " << val4 << "\n";

//    auto val5 = er.convert("nok", "dkk", 1);
//    cerr << "val5: " << val5 << "\n";

//    auto val6 = er.convert("nok" ,"dkk", 10);
//    cerr << "val6: " << val6 << "\n";

    EXPECT_EQ(true, fabs(er.to_eur("dkk", 1) - 0.13437f) < 0.01f);
    EXPECT_EQ(true, fabs(er.to_eur("dkk", 10) - 1.3437f) < 0.01f);

    EXPECT_EQ(true, fabs(er.from_eur("dkk", 1) - 7.4419f) < 0.01f);
    EXPECT_EQ(true, fabs(er.from_eur("dkk", 10) - 74.419f) < 0.01f);

    EXPECT_EQ(true, fabs(er.convert("dkk", "nok", 1)  - 1.41749f) < 0.01f);
    EXPECT_EQ(true, fabs(er.convert("dkk", "nok", 10) - 14.1749f) < 0.01f);

    EXPECT_EQ(true, fabs(er.convert("nok", "dkk", 1)  - 0.705474f) < 0.01f);
    EXPECT_EQ(true, fabs(er.convert("nok", "dkk", 10) - 7.05474f) < 0.01f);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
