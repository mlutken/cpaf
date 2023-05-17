#include <iostream>
#include <vector>
#include <string>
#include <codecvt>
#include <locale>
#include <iomanip>
#include <bitset>
#include <memory>
#include <thread>
#include <filesystem>
#include <sparsehash/dense_hash_map>

#include <streams/cpaf_streams.h>
#include <filesystem/cpaf_special_dirs.h>
#include <unicode/cpaf_u16string_utils.h>
#include <semantic/cpaf_semantic.h>
#include <semantic/primitives/pattern_matcher.h>
#include <semantic/fragments/fragment_matcher.h>
#include <semantic/classifier/fragments_classifier.h>
#include <semantic/readers/json_classification_data_reader.h>
#include <semantic/config/configuration.h>

using json = nlohmann::json;
namespace fs = std::filesystem;
namespace cfs = cpaf::filesystem;
namespace cs = cpaf::streams;
namespace con = cpaf::containers;
namespace cu = cpaf::unicode;

using namespace std;
using namespace cpaf::unicode::osops;
using namespace cpaf::semantic;

using namespace std::chrono_literals;

static const fs::path data_dir = fs::path(__FILE__).parent_path() / "data";
static const fs::path html1 = data_dir / "scott-spark-rc-900-comp-2021.html";
static const fs::path html2 = data_dir / "eightshot-x-coady-275-disc-mtb-275-grey.html";


std::u16string to_dom_tree(const std::u16string& html, bool simplify_ws = true)
{
    const std::vector<std::u16string> to_remove {u"<", u">", u"=", u"\""};
    const std::vector<std::u16string> to_replace {u" ", u" ", u" ", u" "};
    std::u16string s = cu::replace_all_copy(html, to_remove, to_replace);
    if (simplify_ws) {
        return cu::simplify_white_space_copy(s);
    }
    return s;
}

std::u16string to_dom_tree(const std::string& html, bool simplify_ws = true)
{
    return to_dom_tree(cu::to_u16string(html), simplify_ws);

}

std::u16string g_html = uR"(
h1 class product-detail-name mx-auto
SCOTT Spark RC 900 Comp 2021
                    ged
/h1
        hest
)";



int main()
{
    cerr << "--- domtree playground ---\n";
    fragments_classifier classifier;
//    classifier.dbg_mode(true);
    classifier.add(u"id productschema div itemprop name"                            , u"dom_pos", u"item_title__ItemPage", 40);
    classifier.add(u"itemprop name {{*}} /span /h1"                                 , u"dom_pos", u"item_title__ItemPage", 40);
    classifier.add(u"h1 {{*}} product-name {{*}} /h1"          , u"dom_pos", u"item_title__ItemPage", 40);

    source_fields fields;
    fields.extended_debug_mode(true);
//    fields.add(u"html", to_dom_tree(g_html));
    fields.add(u"html", to_dom_tree(cs::to_string(html2)) );

    cs::to_file(fs::path("/tmp/_dom-tree.html"), cu::to_string(to_dom_tree(cs::to_string(html2), false)));
    cerr << "*** classifier.classify() ***\n";

    using record_t = std::unordered_map<std::u16string, std::u16string>;
    record_t rec;
    classifier.detect_values(rec, fields, parse_mode::longest_sentence_multi_pass);
//    classifier.classify(fields, sentence_matcher::parse_mode::longest_full_sentence);
    cerr << "--- scores ---\n";
//    cerr << fields.scores().debug_string() << "\n";
    cerr << fields.scores().extended_debug_string() << "\n";
    cerr << "--- detected values ---\n";
    for (const auto& [key, val]: rec) {
        cerr << key << ": " << val << "\n";
    }
    return 0;
}
