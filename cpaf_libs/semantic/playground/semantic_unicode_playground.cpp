#include <iostream>
#include <vector>
#include <string>
#include <codecvt>
#include <locale>
#include <iomanip>
#include <bitset>

#include <semantic/cpaf_semantic.h>
#include <nlohmann/json.hpp>

// https://en.cppreference.com/w/cpp/string/multibyte/c16rtomb
// https://en.cppreference.com/w/cpp/locale/wstring_convert/from_bytes
// https://en.cppreference.com/w/cpp/locale/wstring_convert/to_bytes
// Stemming: https://github.com/OleanderSoftware/OleanderStemmingLibrary
// https://marknelson.us/posts/1996/08/01/suffix-trees.html
// https://en.wikipedia.org/wiki/Perfect_hash_function
// https://stackoverflow.com/questions/60623196/can-i-create-unordered-map-for-size-of-integer-upto-109

/*


You can use bitset, size 2^27 bytes (128mb). Bit 1 in the appropriate index will define "element within set", and bit 0 will be "element not in set". Such set can be easy written as class like:

class bitset1G {
  public:
  bitset1G() { bzero(_data, sizeof(_data)); }
  void set(uint32_t x) { _data[x >> 5] |= (1 << (x & 0x1f)); }
  void clr(uint32_t x) { _data[x >> 5] &= ~(1 << (x & 0x1f)); }
  bool tst(uint32_t x) { return _data[x >> 5] & (1 << (x & 0x1f)); }
  private:
  int32_t _data[1 << 25]; // 1G bits
};


*/


using json = nlohmann::json;

using namespace std;

class bitset_1G {
  public:
  bitset_1G() { bzero(_data, sizeof(_data)); }
  void set(uint32_t x) { _data[x >> 5] |= (1 << (x & 0x1f)); }
  void clr(uint32_t x) { _data[x >> 5] &= ~(1 << (x & 0x1f)); }
  bool tst(uint32_t x) { return _data[x >> 5] & (1 << (x & 0x1f)); }
  private:
  int32_t _data[1 << 25]; // 1G bits
};

using bitset_1g = std::bitset<(1<<25)*32>;
using bitset_65k = std::bitset<0xFFFF>;

// About the reinterpret_cast<const char*>(u8"grüssEN æøå ÆØÅ Δ") needed in C++20 see
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1423r2.html


int main()
{
    cerr << "--- semantic playground ---\n";
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> cvt_u16;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt_u32;

    std::string str = reinterpret_cast<const char*>("ABC ÆØÅ😀");
    std::string u8str = reinterpret_cast<const char*>(u8"ABC ÆØÅ😀");
    std::u16string u16str = u"ABC ÆØÅŵ";
    std::u32string u32str = U"ABC ÆØÅŵ";
    u16str.push_back(0x00e6);
    u16str.push_back(0x0175);

    u32str.push_back(0x000000e6);
    u32str.push_back(0x0175);

    u16str += cvt_u16.from_bytes(std::string(reinterpret_cast<const char*>(u8" from_bytes æøåÆØÅê")));
    u32str += cvt_u32.from_bytes(std::string(reinterpret_cast<const char*>(u8" from_bytes æøåÆØÅê😀")));


    cerr <<  "str.size()    : " << str.size() * sizeof(std::string::value_type) << " len: " << str.length() << "\n";
    cerr <<  "u8str.size()  : " << u8str.size() * sizeof(std::string::value_type) << " len: " << u8str.length() << "\n";
    cerr <<  "u16str.size() : " << u16str.size() * sizeof(std::u16string::value_type) << " len: " << u16str.length() << "\n";
    cerr <<  "u32str.size() : " << u32str.size() * sizeof(std::u32string::value_type) << " len: " << u32str.length() << "\n";

    cerr <<  "str    : " << str << "\n";
    cerr <<  "u8str  : " << u8str << "\n";
    cerr <<  "u16str : " << cvt_u16.to_bytes(u16str) << "\n";
    cerr <<  "u32str : " << cvt_u32.to_bytes(u32str) << "\n";

    std::string u8json_str = reinterpret_cast<const char*>(u8R"({"key": "valueŵ" })");
    std::u16string u16json_str = uR"({"key": "valueŵ" })";

    auto jobject8 = json::parse(u8json_str);
    auto jobject16 = json::parse(u16json_str);
    cerr << "json8:\n" << std::setw(4) << jobject8 << "\n";

    cerr <<  "u16json_str : " << cvt_u16.to_bytes(u16json_str) << "\n";

    cerr << "json8::key = " << jobject8["key"] << "\n";
    cerr << "json16::key = " << jobject16["key"] << "\n";

    std::string s8 = jobject8["key"];

    cerr << "sizeof(bitset_1G) : " << sizeof(bitset_1G)/1e6  << " MBytes\n";
    cerr << "sizeof(bitset_1g) : " << sizeof(bitset_1g)/1e6  << " MBytes\n";
    cerr << "sizeof(bitset_65k): " << sizeof(bitset_65k)/1e3 << " KBytes\n";
    cerr << "2^25: " << (1 << 25)*32 << "\n";



    return 0;
}

