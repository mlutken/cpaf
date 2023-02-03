
#include <iostream>
#include <QString>
#include "jsonqt.hpp"

//using json = nlohmann::basic_json<std::map, std::vector, std::string, bool, std::int64_t, std::uint64_t,double, std::allocator, nlohmann::StdStringPolicy>;
using json = nlohmann::basic_json<std::map, std::vector, QString, bool, std::int64_t, std::uint64_t, double, std::allocator, nlohmann::QStringPolicy>;

inline json operator "" _json(const char* s, std::size_t)
{
    return json::parse(json::convert_string_litteral(s));
}

inline std::ostream& operator <<(std::ostream& os, const QString& str)
{
    os << str.toUtf8().constData();
    return os;
}

using namespace std;

int main()
{
    json j;
    j["attrib1"] = "val1";
    j["dnum"] = 12.12;
    j["inum"] = 5;
    auto s = j.dump(4);
    cout << s << endl;

    cout << "j['attrib1']: " << j["attrib1"] << endl;


    QString qs("hest");
    qs[0] = QChar('t');

    cout << qs << endl;

    json j2 = {
      {"pi", 3.141},
      {"happy", true},
      {"name", "Niels"},
      {"nothing", nullptr},
      {"answer", {
        {"everything", 42}
      }},
      {"list", {1, 0, 2}},
      {"object", {
        {"currency", "USD"},
        {"value", 42.99}
      }}
    };


    auto s2 = j2.dump(4);
    cout << s2 << endl;

    for (auto& element : j2) {
      std::cout << element << '\n';
    }

    // Not working!
    json j3 = "{ \"happy\": true, \"pi\": 3.141 }"_json;
    auto s3 = j3.dump(4);
    cout << s3 << endl;

    auto j4 = json::parse("{ \"happy\": \"yes\", \"pi\": 3.141 }");

    cout << "j4['happy']: " << j4["happy"] << endl;

    return 0;
}
