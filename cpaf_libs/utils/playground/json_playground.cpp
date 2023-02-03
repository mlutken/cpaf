#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <utils/cpaf_json_utils.h>
#include <nlohmann/json.hpp>


using namespace std;
using json = nlohmann::json;

void parseJsonRecursive(const nlohmann::json& object, int indention);

static const std::string g_PLAYGROUND_json = R"JSON(
{
  "test_bool_false": false,
  "test_bool_true": true,
  "test_number_int_positive": 423,
  "test_number_int_negative": -123,
  "test_number_float": 99.45,
  "test_null": null,
  "@context": "http://schema.org/",
  "@type": "Product",
  "brand": {
      "@type": "Thing",
      "name": "Aqua-Speed"
  },
  "description": "Svømmefødder i farverne grå, ",
  "image": [
      "image 1",
      "image 2",
      "image 3"
  ],
  "name": "Svømmefødder, str. 36/37, 38/39 - Inox, gul (Aqua-Speed)",
  "offers": [
      {
          "@type": "Offer",
          "availability": "http://schema.org/InStock",
          "price": "260.0",
          "priceCurrency": "DKK",
          "sku": "100555",
          "url": "https://www.badehaette.dk/products/svommefodder-str-36-37-38-39-inox-gul-aqua-speed?variant=13457941790833"
      },
      {
          "@type": "Offer",
          "availability": "http://schema.org/InStock",
          "price": "260.0",
          "priceCurrency": "DKK",
          "sku": "100556",
          "url": "https://www.badehaette.dk/products/svommefodder-str-36-37-38-39-inox-gul-aqua-speed?variant=13457941823601"
      }
  ],
  "urlX": "XXX https://www.badehaette.dk/products/svommefodder-str-36-37-38-39-inox-gul-aqua-speed"
}
)JSON";



int main()
{
    cout << "--- json playground ---\n";

    json o;
    o.push_back("one");
    o.push_back("two");
    o.push_back("thre");
    o.push_back("four");
    // special iterator member functions for objects
    if (o.is_array()) {
        for (json::iterator it = o.begin(); it != o.end(); ++it) {
          std::cout << it.value() << "\n";
        }
    }

    return 0;
}


int main1()
{
    cout << "--- json playground ---\n";


    const auto trimmed = boost::algorithm::replace_all_copy( g_PLAYGROUND_json, std::string("\n"), std::string(" ") );
//                        const auto trimmed = trimDefault_fun(value);
    nlohmann::json json_object = json::parse(trimmed);

    parseJsonRecursive(json_object, 0);
    return 0;
}

void outputIndention(int indention)
{
    for (auto i = indention; i >= 0; --i) {
        std::cerr << "    ";
    }
}

void parseJsonRecursive(const nlohmann::json& object, int indention)
{
    if (object.is_object()){
        for (auto& item : object.items()) {
            outputIndention(indention);
            std::cerr << item.key() << '\n';
            parseJsonRecursive(item.value(), indention + 1);
        }
    }
    else if (object.is_array()){
        for (auto& item : object.items()) {
            parseJsonRecursive(item.value(), indention + 1);
        }
    }
    else {
        outputIndention(indention);
        std::string val = cpaf::simple_json_to_string(object);
        std::cerr << val << '\n';
    }
}

void parseJsonRecursiveOLD(const nlohmann::json& object)
{
    for (auto& item : object.items()) {
        if (item.value().is_structured()) {
            parseJsonRecursiveOLD(item.value());
        }
        else {
            const std::string value = item.value().dump();
        }
    }
}

