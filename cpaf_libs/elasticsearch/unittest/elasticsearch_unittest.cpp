#include <iostream>
#include <memory>
#include <thread>
#include <gtest/gtest.h>
#include <boost/filesystem/path.hpp>
#include <nlohmann/json.hpp>
#include <elasticsearch/es_utils.h>
#include <elasticsearch/es_connection.h>
#include <streams/cpaf_streams.h>

using namespace testing;
using namespace std;
using json = nlohmann::json;
namespace fs = boost::filesystem;
namespace cs = cpaf::streams;
namespace con = cpaf::containers;
namespace es = cpaf::elasticsearch;
using namespace std::chrono_literals;

// https://github.com/google/googletest/blob/master/googletest/docs/faq.md

class ElasticSearchUnitTest : public testing::Test
{
public:
    ElasticSearchUnitTest()
    {
    }

    ~ElasticSearchUnitTest() override = default;

    void SetUp() override
    {
        //WEBCC_LOG_INIT("", webcc::LOG_CONSOLE);
    }

    void TearDown() override
    {
    }
};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------
#define RUN_ES_SERVER_TESTS 1

TEST_F(ElasticSearchUnitTest, scroll_id_get__test)
{
    const string search_in1 = R"("_scroll_id" : "DnF1ZXJ5VGhlbkZldGNoBQAAAAAAIfi5FjE5eWd", )";
    auto scroll_id1 = es::response_scroll_id(search_in1);
    EXPECT_EQ("DnF1ZXJ5VGhlbkZldGNoBQAAAAAAIfi5FjE5eWd", scroll_id1);

    const string search_in2 = R"("_scroll_id":"DnF1ZXJ5VGhlbkZldGNoBQAAAAAAIfi5FjE5eWd", )";
    auto scroll_id2 = es::response_scroll_id(search_in2);
    EXPECT_EQ("DnF1ZXJ5VGhlbkZldGNoBQAAAAAAIfi5FjE5eWd", scroll_id1);
}


#if RUN_ES_SERVER_TESTS

es::es_connection get_es()
{
    return es::es_connection{};
}


static const std::string match_all = es::match_all_body;

TEST_F(ElasticSearchUnitTest, append_to_bulk_request__test)
{
const string rec_data = R"(
                        {
                            "currency_code": "dkk",
                            "item_price": "197",
                            "item_title": "Scott Addict 10"
                        }
                        )";

const string expected_bulk_data =
R"({ "index": { "_index" : " cycling_products", "_type": "product", "_id": "scott_addict_10" } }
{"currency_code":"dkk","item_price":"197","item_title":"Scott Addict 10"}
)";

    std::string bulk_data;
    json rec = json::parse(rec_data);
    es::append_to_bulk_request(bulk_data, rec, " cycling_products", "product", "scott_addict_10");
    EXPECT_EQ(expected_bulk_data, bulk_data);
}


TEST_F(ElasticSearchUnitTest, bulk_api__test)
{
    const std::string index = "myproject_bulk";
    auto es = get_es();
    es.delete_index(index);
    EXPECT_EQ(false, es.index_exist(index));
    es.create_index(index, "");
    EXPECT_EQ(true, es.index_exist(index));
    const std::string bulk_data = R"(
{ "index" : { "_index" : "myproject_bulk", "_type": "mytype", "_id" : "1" } }
{ "field" : "value1" }
{ "index" : { "_index" : "myproject_bulk", "_type": "mytype", "_id" : "2" } }
{ "field" : "value2" }
{ "index" : { "_index" : "myproject_bulk", "_type": "mytype", "_id" : "3" } }
{ "field" : "value3" }
{ "index" : { "_index" : "myproject_bulk", "_type": "mytype", "_id" : "4" } }
{ "field" : "value4" }
)";
    es.post("/_bulk", 0, bulk_data);
    std::this_thread::sleep_for(50ms);

    auto jo1 = json::parse(es.read_record("/myproject_bulk/mytype/1"));
    EXPECT_EQ("value1"s, jo1["_source"]["field"]);
    auto jo2 = json::parse(es.read_record("/myproject_bulk/mytype/2"));
    EXPECT_EQ("value2"s, jo2["_source"]["field"]);
    auto jo3 = json::parse(es.read_record("/myproject_bulk/mytype/3"));
    EXPECT_EQ("value3"s, jo3["_source"]["field"]);
    auto jo4 = json::parse(es.read_record("/myproject_bulk/mytype/4"));
    EXPECT_EQ("value4"s, jo4["_source"]["field"]);

    // Cleanup
    es.delete_index(index);
    EXPECT_EQ(false, es.index_exist(index));
}


TEST_F(ElasticSearchUnitTest, indices_functions__test)
{
    const std::string index = "myproject_products";
    const fs::path myproject_index_data_path = fs::path(__FILE__).parent_path().parent_path() / "_data/default_product_site_es_mappings.json";
    const auto create_data = cpaf::streams::to_string(myproject_index_data_path);
    auto es = get_es();
    es.delete_index(index);
    EXPECT_EQ(false, es.index_exist(index));
    es.create_index(index, create_data);
    EXPECT_EQ(true, es.index_exist(index));

    // Cleanup
    es.delete_index(index);
    EXPECT_EQ(false, es.index_exist(index));
}


/*
curl -XGET "localhost:9200/customer/external/1?pretty=1" -H 'Content-Type: application/json'
*/
TEST_F(ElasticSearchUnitTest, single_record_read_write__test)
{
    const std::string test_base_path = "/record_read_write_testindex/mytesttype";
    auto es = get_es();
    const std::string customer1 = R"(
    {
      "name": "John Dalton"
    }
    )";

    EXPECT_EQ(true, es.write_record(test_base_path + "/1", customer1));
    auto jo = json::parse(es.read_record(test_base_path +"/1"));
    EXPECT_EQ("John Dalton", jo["_source"]["name"]);
    jo["_source"]["name"] = "John Doe";

    EXPECT_EQ(true, es.write_record(test_base_path + "/1", jo["_source"].dump()));
    jo = json::parse(es.read_record(test_base_path + "/1"));
    EXPECT_EQ("John Doe", jo["_source"]["name"]);
}

TEST_F(ElasticSearchUnitTest, scroll__test)
{
    const std::string test_base_path = "/scroll_testindex/mytesttype";
    auto es = get_es();
    const std::vector<std::string> customers = {
        R"( { "name": "Anders And"    , "number" : 1 } )",
        R"( { "name": "Brian Børgesen", "number" : 2  } )",
        R"( { "name": "Carl Custer", "number" : 3  } )",
        R"( { "name": "Dorte Døgenigt", "number" : 4  } )",
        R"( { "name": "Egon Ejlersen", "number" : 5  } )",
        R"( { "name": "Finn Fjot", "number" : 6  } )"
    };

    // Write some records
    for (size_t n = 0; n < customers.size(); ++n ) {
        EXPECT_EQ(true, es.write_record(test_base_path + "/"  + std::to_string(n), customers[n]));
    }
    std::this_thread::sleep_for(50ms);

    es.pretty(false);
    // Use scroll API to read the records
    auto r = es.scroll_post(test_base_path + "/_search", 2, "2m", match_all);
    bool more_data = es::response_has_hits(r->data());
    EXPECT_EQ(true, more_data);
    EXPECT_EQ(6, es::response_hit_count(r->data()));

    std::set<int> customer_numbers;
    int es_request_counter = 0;
    while (more_data) {
        es_request_counter++;
        if (es_request_counter > 4) break;
        auto jo = json::parse(r->data());
        EXPECT_EQ(2, jo["hits"]["hits"].size());
        for (const auto& es_rec : jo["hits"]["hits"]) {
            const auto& rec = es_rec["_source"];
            const int number = rec["number"];
            customer_numbers.insert(number);
        }
        r = es.scroll_post_next();
        more_data = cpaf::elasticsearch::response_has_hits(r->data());
    }
    EXPECT_EQ(3, es_request_counter);
    const std::set<int> expected_customer_numbers = {1,2,3,4,5,6};
    EXPECT_EQ(expected_customer_numbers, customer_numbers);
}

#endif //RUN_ES_SERVER_TESTS

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
