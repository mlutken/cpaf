#include <iostream>
#include <memory>
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
#include <semantic/primitives/word_classes_lookup.h>
#include <semantic/classifier/fragments_classifier.h>
#include <cpaf_libs/semantic/readers/json_classification_data_reader.h>

using namespace testing;
using namespace std;
using namespace cpaf::semantic;

using record_t = std::unordered_map<std::u16string, std::u16string>;

class FragmentsClassifierUnitTest : public testing::Test
{
public:
    FragmentsClassifierUnitTest()
    {
    }

    ~FragmentsClassifierUnitTest() override = default;

    void SetUp() override
    {
        classifier_ = std::make_unique<fragments_classifier>();
        classifier_->add(u"giro", u"brand_name", u"giro", 20);
        classifier_->add(u"bmx cykel", u"item_type", u"bmx_bike", 20);
        classifier_->add(u"dame", u"item_type", u"city_bike", 5);
        classifier_->add(u"dame", u"sex", u"f", 20);
        classifier_->add(u"damecykel", u"item_type", u"city_bike", 25);
        classifier_->add(u"damecykel", u"sex", u"f", 20);
        classifier_->add(u"dame cykel", u"item_type", u"city_bike", 25);
        classifier_->add(u"dame cykel", u"sex", u"f", 20);
        classifier_->add(u"taske til bagagebærer", u"my_type", u"my_bags_component", 40);
        classifier_->add(u"taske {{~}} bagagebærer", u"my_type", u"my_bags_component", 30);
        classifier_->add(u"taske {{*}} bagagebærer", u"my_type", u"my_bags_component", 20);
        classifier_->add(u"easydo ed-2602");
        classifier_->add(u"cykel");
        classifier_->add(u"herrecykel");
        classifier_->add(u"damecykl*", u"item_type", u"city_bike", 30, u"breadcrumbs");
        classifier_->add(u"damecykl*", u"sex", u"f", 22, u"breadcrumbs");

        classifier_->add(u"tricykel", u"item_type", u"tri_bike", 60);
        classifier_->add(u"trehjulet cykel", u"item_type", u"learner_bike", 30);
        classifier_->add(u"trehjulet cykel", u"item_type", u"tri_bike", -100);
        classifier_->add(u"trehjulet cykel", u"kids", u"y", 50);
        classifier_->add(u"sort", u"color_property", u"black", 25);
        classifier_->add(u"blå", u"color_property", u"blue", 30);
        classifier_->add(u"tri/enkeltstart", u"item_type", u"tri_bike", 100);
    }

    void TearDown() override
    {
    }

    std::string cycling_bike_type_json      () const;
    std::string cycling_clothes_type_json   () const;
    std::string cycling_component_type_json () const;

    std::unique_ptr<fragments_classifier> classifier_;

};

// ------------------------------
// --- PUBLIC functions tests ---
// ------------------------------

TEST_F(FragmentsClassifierUnitTest, test_basic1)
{
    record_t item_record;
    source_fields fields;
    fields.add(u"item_title", u"giro bmx cykel dame taske noget mere bagagebærer");
    classifier_->detect_values(item_record, fields, parse_mode::longest_sentence);

    EXPECT_EQ(4, item_record.size());
    EXPECT_EQ(u"bmx_bike"           , item_record[u"item_type"]);
    EXPECT_EQ(u"giro"               , item_record[u"brand_name"]);
    EXPECT_EQ(u"my_bags_component"  , item_record[u"my_type"]);
    EXPECT_EQ(u"f"                  , item_record[u"sex"]);
    EXPECT_EQ(20                    , fields.scores().score(u"item_type", u"bmx_bike"));
    EXPECT_EQ(5                     , fields.scores().score(u"item_type", u"city_bike"));
    EXPECT_EQ(20                    , fields.scores().score(u"brand_name", u"giro"));
    EXPECT_EQ(20                    , fields.scores().score(u"sex", u"f"));
    EXPECT_EQ(20                    , fields.scores().score(u"my_type", u"my_bags_component"));
}


/** Separators 1 */
TEST_F(FragmentsClassifierUnitTest, test_separators1)
{
    record_t item_record;
    source_fields fields;
    fields.add(u"item_title", u"cykel/sort. til tri/enkeltstart");
    classifier_->detect_values(item_record, fields, parse_mode::longest_sentence);

    EXPECT_EQ(2, item_record.size());
    EXPECT_EQ(u"black"          , item_record[u"color_property"]);
    EXPECT_EQ(u"tri_bike"       , item_record[u"item_type"]);
    EXPECT_EQ(100               , fields.scores().score(u"item_type", u"tri_bike"));
    EXPECT_EQ(25                , fields.scores().score(u"color_property", u"black"));
}




/** Separators 2 */
TEST_F(FragmentsClassifierUnitTest, test_separators2)
{
    record_t item_record;
    source_fields fields;
    fields.add(u"item_title", u"trehjulet cykel/blå.");
    classifier_->detect_values(item_record, fields, parse_mode::longest_sentence);

    EXPECT_EQ(3, item_record.size());
    EXPECT_EQ(u"blue"               , item_record[u"color_property"]);
    EXPECT_EQ(u"y"                  , item_record[u"kids"]);
    EXPECT_EQ(u"learner_bike"       , item_record[u"item_type"]);
    EXPECT_EQ(30                    , fields.scores().score(u"item_type", u"learner_bike"));
    EXPECT_EQ(50                    , fields.scores().score(u"kids", u"y"));
    EXPECT_EQ(30                    , fields.scores().score(u"color_property", u"blue"));
}

/** Negative weights */
TEST_F(FragmentsClassifierUnitTest, test_negative_weights)
{
    record_t item_record;
    source_fields fields;
    fields.add(u"item_title", u"winther trehjulet cykel tricykel");
    classifier_->detect_values(item_record, fields, parse_mode::longest_sentence);

    EXPECT_EQ(2, item_record.size());
    EXPECT_EQ(u"y"                  , item_record[u"kids"]);
    EXPECT_EQ(u"learner_bike"       , item_record[u"item_type"]);
    EXPECT_EQ(30                    , fields.scores().score(u"item_type", u"learner_bike"));
    EXPECT_EQ(-40                   , fields.scores().score(u"item_type", u"tri_bike"));
    EXPECT_EQ(50                    , fields.scores().score(u"kids", u"y"));
}


/** Test that we can have one word/fragment classify multiple types */
TEST_F(FragmentsClassifierUnitTest, test_multi_type_classification1)
{
    record_t item_record;
    source_fields fields;
    fields.add(u"item_title", u"damecykel taske til bagagebærer");
    classifier_->detect_values(item_record, fields, parse_mode::longest_sentence);

    EXPECT_EQ (3, item_record.size());
    EXPECT_EQ(u"f"                  , item_record[u"sex"]);
    EXPECT_EQ(u"city_bike"          , item_record[u"item_type"]);
    EXPECT_EQ(25                    , fields.scores().score(u"item_type", u"city_bike"));
    EXPECT_EQ(20                    , fields.scores().score(u"sex", u"f"));
    EXPECT_EQ(u"my_bags_component"  , item_record[u"my_type"]);
    EXPECT_EQ(40                    , fields.scores().score(u"my_type", u"my_bags_component"));
}

/** Test that we can have one word/fragment classify multiple types */
TEST_F(FragmentsClassifierUnitTest, test_multi_type_classification2)
{
    record_t item_record;
    source_fields fields;
    fields.add(u"item_title", u"dame cykel med taske til bagagebærer");
    classifier_->detect_values(item_record, fields, parse_mode::longest_sentence);

    EXPECT_EQ (3, item_record.size());
    EXPECT_EQ(u"f"                  , item_record[u"sex"]);
    EXPECT_EQ(u"city_bike"          , item_record[u"item_type"]);
    EXPECT_EQ(25                    , fields.scores().score(u"item_type", u"city_bike"));
    EXPECT_EQ(20                    , fields.scores().score(u"sex", u"f"));
    EXPECT_EQ(u"my_bags_component"  , item_record[u"my_type"]);
    EXPECT_EQ(40                    , fields.scores().score(u"my_type", u"my_bags_component"));
}

/** Test that we can classify using multiple source fields */
TEST_F(FragmentsClassifierUnitTest, test_multi_fields_classification1)
{
    record_t item_record;
    source_fields fields;
    fields.add(u"item_title", u"sej dame cykel med taske og bagagebærer");
    fields.add(u"breadcrumbs", u"cykler -> damecykler");
    classifier_->detect_values(item_record, fields, parse_mode::longest_sentence);

    EXPECT_EQ (3, item_record.size());
    EXPECT_EQ(u"f"                  , item_record[u"sex"]);
    EXPECT_EQ(u"city_bike"          , item_record[u"item_type"]);
    EXPECT_EQ(55                    , fields.scores().score(u"item_type", u"city_bike"));
    EXPECT_EQ(42                    , fields.scores().score(u"sex", u"f"));
    EXPECT_EQ(u"my_bags_component"  , item_record[u"my_type"]);
    EXPECT_EQ(30                    , fields.scores().score(u"my_type", u"my_bags_component"));
}


#if 0

/** fragments override with negative weights test
 * @todo Implement this!
 */
TEST_F(FragmentsClassifierUnitTest, test_xxx)
{
    const classification_fragment_reader_base::weights_map global_weights =
    {
        {"*", 15},
        {"breadcrumbs_text", 20},
        {"classification_text", 16},
        {"description_text", 8},
        {"item_subtype_text", 80},
        {"item_title", 40},
        {"detect_master_name", 60},
        {"item_title_wheel_size", 41},
        {"item_title_leftover_text", 42},
        {"item_type_text", 100},
        {"wheel_size_text", 43},
        {"groupset", 44},
        {"frame_material_text", 45},
        {"gear_count_text", 46}
    };

    fragments_classifier classifier(u"da");
    json_classification_data_reader r1(cycling_clothes_type_json());
    json_classification_data_reader r2(cycling_component_type_json());
    r1.global_weights(global_weights);
    r2.global_weights(global_weights);
//    classifier.add_fragments(r1, u"item_type");
    classifier.add_fragments(r2, u"item_type");
    cerr << "language_code: '" << r1.language_code() << "'\n";

    nlohmann::json rec1;
    source_fields f1;
    f1.extended_debug_mode(true);
    f1.add("item_title", "briller");
    classifier.detect_values_str(rec1, f1, parse_mode::longest_full_sentence);
    cerr << rec1.dump(4) << "\n";
    cerr << f1.dbg_string() << "\n";

    EXPECT_EQ(25                , 25);
}


std::string FragmentsClassifierUnitTest::cycling_bike_type_json () const
{
    return "";
}

std::string FragmentsClassifierUnitTest::cycling_clothes_type_json () const
{
    return R"json(
    {
     "default_weights" : {
         "*" : "11"
         ,"item_title" : "41"
     },
     "names_da_add_from_language": [
         "en"
     ],
     "values":
     [
         {
           "item_family": "cycling_clothes",
           "item_group": "clothes_group",
           "item_type": "glasses_clothes",
           "names_da": [
               "brille",
               "briller"
           ],
           "type": "clothes_type",
           "val": "glasses_clothes"
         }
     ]
    }
)json"s;

}

std::string FragmentsClassifierUnitTest::cycling_component_type_json () const
{
    return R"json(
    {
     "default_weights" : {
           "*" : "20"
          ,"item_title" : "22"
     },
     "names_da_add_from_language": [
         "en"
     ],
     "values":
     [
         {
           "component_group": "frames_group",
           "item_family": "cycling_component",
           "item_group": "frames_group",
           "item_type": "frames_component",
           "names_da": [
               "[*:-200] briller"
           ],
           "type": "component_type",
           "val": "frames_component"
         }
     ]
    }
)json"s;
}

#endif


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
