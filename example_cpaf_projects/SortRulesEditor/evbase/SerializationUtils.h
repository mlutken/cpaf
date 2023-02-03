// emacs: -*- Mode: c++; c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
// jed:   -*- Mode: C; eval: c_set_style("bsd") -*-
#ifndef EVBASE_SERIALIZATIONUTILS_H
#define EVBASE_SERIALIZATIONUTILS_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include <QString>

namespace evbase {
    namespace details {
        std::string escapeValue(const std::string &s);
        QString escapeValue(const QString &s);
    } // anonymous details

    /** Create comma separated std::string from container/range. Separator can be any char.*/
    template<typename RANGE_TYPE>
    std::string containerToString(const RANGE_TYPE &v, char sep = ',', bool escape_values = true)
    {
        std::string s;
        const auto it_begin = std::begin(v);
        const auto it_end = std::end(v);
        for (auto it = it_begin; it != it_end; ++it) {
            if (it != it_begin)
                s += sep;

            if (escape_values)
                s += details::escapeValue(*it);
            else
                s += *it;

        }
        return s;
    }

    /** Create comma separated QString from container/range. Separator can be any char.*/
    template<typename RANGE_TYPE>
    QString containerToQString(const RANGE_TYPE &v, char sep = ',', bool escape_values = true)
    {
        QString s;
        const auto it_begin = std::begin(v);
        const auto it_end = std::end(v);
        for (auto it = it_begin; it != it_end; ++it) {
            if (it != it_begin)
                s += sep;

            if (escape_values)
                s += details::escapeValue(*it);
            else
                s += *it;

        }
        return s;
    }

    std::string stringMapToString(const std::map<std::string, std::string> &m, char sep = ',', bool escape_values = true );
    std::map<std::string, std::string> stringMapFromString(const std::string &str, char sep = ',', bool trim_elements = false);

    std::string stringVectorToString(const std::vector<std::string> &v, char sep = ',', bool escape_values = true);
    std::vector<std::string> stringVectorFromString(const std::string &str, char sep = ',', bool trim_elements = false);

    std::string stringSetToString(const std::set<std::string> &set, char sep = ',', bool escape_values = true);
    std::set<std::string> stringSetFromString(const std::string &str, char sep = ',', bool trim_elements = false);


    QString stringMapToQString(const std::map<QString, QString> &m, char sep = ',', bool escape_values = true );
    std::map<QString, QString> stringMapFromQString(const QString &str, char sep = ',', bool trim_elements = false);

    QString stringVectorToQString(const std::vector<QString> &v, char sep = ',', bool escape_values = true);
    std::vector<QString> stringVectorFromQString(const QString &str, char sep = ',', bool trim_elements = false);

    QString stringSetToQString(const std::set<QString> &set, char sep = ',', bool escape_values = true);
    std::set<QString> stringSetFromQString(const QString &str, char sep = ',', bool trim_elements = false);

}

#endif
