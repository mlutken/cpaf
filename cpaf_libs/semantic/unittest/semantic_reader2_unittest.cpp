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
    "breadcrumbs_text" : "21",
    "description_text" : "1",
    "item_title" : "41"
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
         "names_da": {
             "cykeltaske": {
                       "breadcrumbs_text":-101,
                       "item_title":201
             },
             "taske til bagagebærer": null
         },
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


class SemanticReader2UnitTest : public testing::Test
{
public:
    SemanticReader2UnitTest()
    {
    }

    ~SemanticReader2UnitTest() override = default;

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
TEST_F(SemanticReader2UnitTest, da__test_reading_from_json_string)
{
    json_classification_data_reader r(json_data);
    r.language_code("da");
    r.read_data();
    r.begin_iterating();
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


TEST_F(SemanticReader2UnitTest, en__test_reading_from_json_string)
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



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
