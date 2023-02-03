#include <iostream>
#include <memory>
#include <string>
#include <gtest/gtest.h>
#include <semantic/readers/json_classification_data_reader.h>

using namespace testing;
using namespace std;
using namespace cpaf::semantic;

// https://github.com/google/googletest/blob/master/googletest/docs/faq.md

const auto json_data = R"(
{
 "default_weights" : {
     "*" : "11",
     "item_title" : "41",
     "description_text" : "1",
     "breadcrumbs_text" : "21"
 },
 "names_da_add_from_language": [
     "en"
 ],
 "values":
 [
     {
         "component_group": "transportation_and_storage_group",
         "item_family": "cycling_component",
         "item_group": "transportation_and_storage_group",
         "item_type": "rack_bags_component",
         "names_da": [
             "taske til bagagebærer",
             "[breadcrumbs_text:-101,item_title:201]  cykeltaske"
         ],
         "names_en": [
            "bike bag"
         ],
         "type": "component_type",
         "val": "rack_bags_component"
     }
     ,
     {
         "component_group": "transportation_and_storage_group",
         "item_family": "cycling_component",
         "item_group": "wheels_group",
         "item_type": "wheels_component",
         "names_da": [
             "cykelhjul"
         ],
         "names_en": [
             "bike wheel"
         ],
         "type": "component_type",
         "val": "wheels_component"
     }

 ]
}
)"s;


class SemanticReaderUnitTest : public testing::Test
{
public:
    SemanticReaderUnitTest()
    {
    }

    ~SemanticReaderUnitTest() override = default;

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
TEST_F(SemanticReaderUnitTest, parse_fragment_weights__test)
{
    std::map<std::string, int> weights;
    std::string fragment = "Bike helmet [*:-3, item_title:6, breadcrumbs_text:4]";
    auto rest = parse_fragment_weights(weights, fragment);

    EXPECT_EQ("Bike helmet", rest);
    EXPECT_EQ(3, weights.size());
    EXPECT_EQ(-3, weights["*"]);
    EXPECT_EQ(6, weights["item_title"]);
    EXPECT_EQ(4, weights["breadcrumbs_text"]);

    fragment = "[*:-3, item_title:6, breadcrumbs_text:4] Bike helmet";
    rest = parse_fragment_weights(weights, fragment);

    EXPECT_EQ("Bike helmet", rest);
    EXPECT_EQ(3, weights.size());
    EXPECT_EQ(-3, weights["*"]);
    EXPECT_EQ(6, weights["item_title"]);
    EXPECT_EQ(4, weights["breadcrumbs_text"]);
}


TEST_F(SemanticReaderUnitTest, en__test_reading_from_json_string)
{
    json_classification_data_reader r(json_data);
    r.language_code("en");
    r.read_data();
    r.begin_iterating();
    EXPECT_EQ(r.weight       ()                 , 11);
    EXPECT_EQ(r.source_field ()                 , "*");
    EXPECT_EQ(r.fragment     ()                 , "bike bag");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 21);
    EXPECT_EQ(r.source_field ()                 , "breadcrumbs_text");
    EXPECT_EQ(r.fragment     ()                 , "bike bag");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 1);
    EXPECT_EQ(r.source_field ()                 , "description_text");
    EXPECT_EQ(r.fragment     ()                 , "bike bag");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 41);
    EXPECT_EQ(r.source_field ()                 , "item_title");
    EXPECT_EQ(r.fragment     ()                 , "bike bag");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");


    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 11);
    EXPECT_EQ(r.source_field ()                 , "*");
    EXPECT_EQ(r.fragment     ()                 , "bike wheel");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 21);
    EXPECT_EQ(r.source_field ()                 , "breadcrumbs_text");
    EXPECT_EQ(r.fragment     ()                 , "bike wheel");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 1);
    EXPECT_EQ(r.source_field ()                 , "description_text");
    EXPECT_EQ(r.fragment     ()                 , "bike wheel");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 41);
    EXPECT_EQ(r.source_field ()                 , "item_title");
    EXPECT_EQ(r.fragment     ()                 , "bike wheel");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");

    r.next_fragment();
    EXPECT_EQ(r.iterating_done() , true);

}


TEST_F(SemanticReaderUnitTest, da__test_reading_from_json_string)
{
    json_classification_data_reader r(json_data);
    r.language_code("da");
    r.read_data();
    r.begin_iterating();
    EXPECT_EQ(r.weight       ()                 , 11);
    EXPECT_EQ(r.source_field ()                 , "*");
    EXPECT_EQ(r.fragment     ()                 , "taske til bagagebærer");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 21);
    EXPECT_EQ(r.source_field ()                 , "breadcrumbs_text");
    EXPECT_EQ(r.fragment     ()                 , "taske til bagagebærer");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 1);
    EXPECT_EQ(r.source_field ()                 , "description_text");
    EXPECT_EQ(r.fragment     ()                 , "taske til bagagebærer");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 41);
    EXPECT_EQ(r.source_field ()                 , "item_title");
    EXPECT_EQ(r.fragment     ()                 , "taske til bagagebærer");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");

    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 11);
    EXPECT_EQ(r.source_field ()                 , "*");
    EXPECT_EQ(r.fragment     ()                 , "cykeltaske");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , -101);
    EXPECT_EQ(r.source_field ()                 , "breadcrumbs_text");
    EXPECT_EQ(r.fragment     ()                 , "cykeltaske");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 1);
    EXPECT_EQ(r.source_field ()                 , "description_text");
    EXPECT_EQ(r.fragment     ()                 , "cykeltaske");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 201);
    EXPECT_EQ(r.source_field ()                 , "item_title");
    EXPECT_EQ(r.fragment     ()                 , "cykeltaske");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");

    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 11);
    EXPECT_EQ(r.source_field ()                 , "*");
    EXPECT_EQ(r.fragment     ()                 , "bike bag");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 21);
    EXPECT_EQ(r.source_field ()                 , "breadcrumbs_text");
    EXPECT_EQ(r.fragment     ()                 , "bike bag");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 1);
    EXPECT_EQ(r.source_field ()                 , "description_text");
    EXPECT_EQ(r.fragment     ()                 , "bike bag");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 41);
    EXPECT_EQ(r.source_field ()                 , "item_title");
    EXPECT_EQ(r.fragment     ()                 , "bike bag");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "rack_bags_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "rack_bags_component");

    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 11);
    EXPECT_EQ(r.source_field ()                 , "*");
    EXPECT_EQ(r.fragment     ()                 , "cykelhjul");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 21);
    EXPECT_EQ(r.source_field ()                 , "breadcrumbs_text");
    EXPECT_EQ(r.fragment     ()                 , "cykelhjul");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 1);
    EXPECT_EQ(r.source_field ()                 , "description_text");
    EXPECT_EQ(r.fragment     ()                 , "cykelhjul");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 41);
    EXPECT_EQ(r.source_field ()                 , "item_title");
    EXPECT_EQ(r.fragment     ()                 , "cykelhjul");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");

    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 11);
    EXPECT_EQ(r.source_field ()                 , "*");
    EXPECT_EQ(r.fragment     ()                 , "bike wheel");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 21);
    EXPECT_EQ(r.source_field ()                 , "breadcrumbs_text");
    EXPECT_EQ(r.fragment     ()                 , "bike wheel");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 1);
    EXPECT_EQ(r.source_field ()                 , "description_text");
    EXPECT_EQ(r.fragment     ()                 , "bike wheel");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");
    r.next_fragment();
    EXPECT_EQ(r.weight       ()                 , 41);
    EXPECT_EQ(r.source_field ()                 , "item_title");
    EXPECT_EQ(r.fragment     ()                 , "bike wheel");
    EXPECT_EQ(r.type         ()                 , "component_type");
    EXPECT_EQ(r.value        ()                 , "wheels_component");
    EXPECT_EQ(r.custom_field ("item_type")      , "wheels_component");

    r.next_fragment();
    EXPECT_EQ(r.iterating_done() , true);

}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
