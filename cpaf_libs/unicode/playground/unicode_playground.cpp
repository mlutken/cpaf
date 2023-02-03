#include <iostream>
#include <vector>
#include <boost/locale.hpp>

#include <unicode/cpaf_u8string_utils.h>
#include <unicode/cpaf_u16string_utils.h>

using namespace std;
using namespace cpaf::unicode::osops;
namespace cu = cpaf::unicode;

// About the reinterpret_cast<const char*>(u8"grüssEN æøå ÆØÅ Δ") needed in C++20 see
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r2.html
std::u16string g_html = uR"(
    <span class="h5 mb-0 pl-2 pr-2 text-center align-self-middle"> pulsure.dk </span>
    <button class="btn btn-light btn-sm border pr-2" style="min-width:230px;" title="Gå til webshop">
         <span class="h6">Gå til webshop</span>
        <img class="align-self-middle h5 mb-0 float-right pl-2" src="/web/images_productsite/general/chevronRight_lightblue.svg" style="height: 21px;" alt="lightblue arrow, go to shop">
    </button>
</a>
)";

int main()
{
    cout << "--- unicode playground ---\n";
    std::u16string item_title1 = u"æøå ÆØÅØÙÚÜÝÞß Δ";
    std::u16string item_title1_lc = cu::to_lower_copy(item_title1);

    std::string item_title2 = reinterpret_cast<const char*>(u8"æøå ÆØÅØÙÚÜÝÞß Δ");

    cerr <<  "item_title1        : " << item_title1 << "\n";
    cerr <<  "item_title1_lc     : " << item_title1_lc << "\n";
    cerr <<  "item_title2        : " << item_title2 << "\n";
    cerr <<  "item_title2_lc (1) : " << cu::to_lower_copy(item_title2) << "\n";

    boost::locale::generator gen;
    std::locale loc=gen("");
    std::locale::global(loc);
    std::cerr.imbue(loc);
    std::string grussen = reinterpret_cast<const char*>(u8"grüssEN æøå ÆØÅ Δ");
    std::string poopla = "\U0001F4A9";
    std::cerr   <<"Upper "<< boost::locale::to_upper(grussen)  << std::endl
                <<"Lower "<< boost::locale::to_lower(grussen)  << std::endl
                <<"Title "<< boost::locale::to_title(grussen)  << std::endl
                <<"Fold  "<< boost::locale::fold_case(grussen) << std::endl
                <<"Poop  "<< boost::locale::to_lower(poopla)   << std::endl;


    const std::string search_in = "find the string between here and the one here in the end";
    std::string::const_iterator from = search_in.begin();
    auto substr = cu::parse_substring_between(from, search_in.end(), "between here", "here");

    cerr << "search_in: '" << search_in << "'\n";
    cerr << "substr: '" << substr << "'\n";                           // Outputs: ' and the one '
    cerr << "rest: '" << std::string(from, search_in.end()) << "'\n"; // Outputs: ' in the end'

    std::u16string n1 = u".Name;   dash-connected traildash- and -predash (GED) farve 1.2 free dash -   before this  : grøn {hval} [hest] traildot. here and here.";
    std::u16string n2 = u"  A   B  \nC    ";
    std::u16string n3 = u"  A $ & \\ B / ; \n  C ";

    cerr << "name_to_id              '" << n1 << "' => '" << cu::name_to_id(n1) << "'\n";
    cerr << "simplify_title          \n'" << n1 << "' => \n'" << cu::simplify_title_copy(n1) << "'\n";
    cerr << "simplify_title (ID)     \n'" << n1 << "' => \n'" << cu::name_to_id(cu::simplify_title_copy(n1)) << "'\n";
    cerr << "simplify_white_space n2 '" << n2 << "' => '" << cu::simplify_white_space_copy(n2) << "'\n";
    cerr << "simplify_white_space n3 '" << n3 << "' => '" << cu::simplify_white_space_copy(n3) << "'\n";
    cerr << "simplify_white_space n3 '" << n3 << "' => '" << cu::simplify_white_space_copy(n3, {u'$', u'&', u'\\', u'/', u';'}, false) << "'\n";


    cerr << "--- html clean playground ---\n";
    cerr << "RAW HTML:\n" << g_html << "\n-----------------------\n";
    cerr << "CLEAN HTML:\n" << cu::clean_html_tags_copy(g_html, cu::post_op::simplify_title) << "\n-----------------------\n";


    return 0;
}

