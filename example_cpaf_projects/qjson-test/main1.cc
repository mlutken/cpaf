
#include <iostream>
#include <QString>
#include "json.hpp"

using my_string_type = QString;
//using my_string_type = std::string;

//using json = nlohmann::basic_json<std::map, std::vector, std::string>;
using json = nlohmann::basic_json<std::map, std::vector, my_string_type>;


inline std::string toStdString(const std::string& str)
{
    return str;
}

inline std::string toStdString(const QString& str)
{
    return str.toUtf8().data();
}

using namespace std;

int main()
{
    json j;
    j["attrib1"] = "val1";
    j["dnum"] = 12.12;
    j["inum"] = 5;

    double dnum = j["dnum"];
    int inum = j["inum"];

    auto v1 = j["attrib1"];
    my_string_type v = j["attrib1"];
    //    QString v = j.get<QString>("attrib1");
//    auto to = typeof(j["attrib1"]);


    cout << "j['attrib1']: " << toStdString(v) << endl;
    cout << "j['dnum']: " << dnum << endl;
    cout << "j['inum']: " << inum << endl;
//    cout << "j['attrib1']: " << qPrintable(v1) << endl;
    auto s = j.dump(4);
//    cout << "\n" << qPrintable(s) << endl;
    return 0;
}
