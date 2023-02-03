// emacs: -*- Mode: c++; c-file-style: "stroustrup"; c-basic-offset: 4; indent-tabs-mode: nil; -*-
// jed:   -*- Mode: C; eval: c_set_style("bsd") -*-
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <QStringList>

#include <evbase/SerializationUtils.h>

using namespace std;
using namespace evbase;


namespace evbase {
    namespace details {
        std::string escapeValue(const std::string &s)
        {
            return "\""
                + boost::replace_all_copy(boost::replace_all_copy(s, "\\", "\\\\"),
                                          "\"", "\\\"") + "\"";
        }

        QString escapeValue(const QString &s)
        {
            QString s1 = s;
            s1 = s1.replace("\\", "\\\\");
            s1 = s1.replace("\"", "\\\"");
            return QString("\"") + s1 + "\"";
        }
    } // anonymous details

    // -----------------------------
    // --- std::string functions ---
    // -----------------------------

    std::string stringMapToString(const std::map<std::string, std::string> &m, char sep, bool escape_values)
    {
        ostringstream ss;

        for (map<string, string>::const_iterator it = m.begin(); it != m.end(); ++it) {
            if (it != m.begin())
                ss << sep;
            if (escape_values)
                ss  << details::escapeValue(it->first) << sep << details::escapeValue(it->second);
            else
                ss  << it->first << sep << it->second;
        }

        return ss.str();
    }

    std::map<std::string, std::string> stringMapFromString(const std::string &str, char sep, bool trim_elements)
    {
        map<string, string> m;
        typedef boost::escaped_list_separator<char> Separator;
        const Separator separator( '\\', sep, '\"');

        boost::tokenizer<Separator> tk(str, separator);
        for (boost::tokenizer<Separator>::iterator it = tk.begin(); it != tk.end();) {
            // get key and value fields
            const string f1 = *it;

            ++it;

            if (it == tk.end())
                throw std::exception();

            const string f2 = *it;
            ++it;

            if (trim_elements) {
                const auto trimmed1 = boost::algorithm::trim_copy(f1);
                const auto trimmed2 = boost::algorithm::trim_copy(f2);
                if (trimmed1 != "" && trimmed2 != "")
                    m[trimmed1] = trimmed2;
            }
            else
                m[f1] = f2;
        }
        return m;
    }

    /** Create comma separated string from vector of strings. */
    std::string stringVectorToString(const std::vector<std::string> &v, char sep, bool escape_values)
    {
        return containerToString(v,sep, escape_values);
    }


    /** Split a comma separated string into a vector of strings. */
    std::vector<std::string> stringVectorFromString(const std::string &str, char sep, bool trim_elements)
    {
        vector<string> vec;
        typedef boost::escaped_list_separator<char> Separator;
        const Separator separator( '\\',  sep, '\"');

        boost::tokenizer<Separator> tk(str, separator);
        if (trim_elements) {
            for (const auto &val : tk) {
                const auto trimmed = boost::algorithm::trim_copy(val);
                if (trimmed != "") vec.push_back(trimmed);
            }
        }
        else {
            for (const auto &val : tk)
                vec.push_back(val);
        }
        return vec;
    }


    /** Create comma separated string from set of strings. */
    std::string stringSetToString(const std::set<std::string> &set, char sep, bool escape_values)
    {
        return containerToString(set,sep, escape_values);
    }

    /** Split a comma separated string into a set of strings. */
    std::set<std::string> stringSetFromString(const std::string &str, char sep, bool trim_elements)
    {
        set<string> set;
        typedef boost::escaped_list_separator<char> Separator;
        const Separator separator( '\\',  sep, '\"');

        boost::tokenizer<Separator> tk(str, separator);
        if (trim_elements) {
            for (const auto &val : tk) {
                const auto trimmed = boost::algorithm::trim_copy(val);
                if (trimmed != "") set.insert(trimmed);
            }
        }
        else {
            for (const auto &val : tk)
                set.insert(val);
        }
        return set;
    }

    // -----------------------------
    // --- QString functions ---
    // -----------------------------

    QString stringMapToQString(const std::map<QString, QString> &m, char sep, bool escape_values)
    {
        QString s;

        for (map<QString, QString>::const_iterator it = m.begin(); it != m.end(); ++it) {
            if (it != m.begin())
                s.append(sep);
            if (escape_values)
                s.append(details::escapeValue(it->first) + sep + details::escapeValue(it->second));
            else
                s.append(it->first + sep + it->second);
        }

        return s;
    }

    std::map<QString, QString> stringMapFromQString(const QString &str, char sep, bool trim_elements)
    {
        map<QString, QString> m;

        auto list = str.split(sep);
        const auto it_end = list.end();
        for (auto it = list.begin(); it != it_end;) {
            // get key and value fields
            const QString f1 = *it;

            ++it;

            if (it == it_end)
                throw std::exception();

            const QString f2 = *it;
            ++it;

            if (trim_elements) {
                const auto trimmed1 = f1.trimmed();
                const auto trimmed2 = f2.trimmed();
                if (trimmed1 != "" && trimmed2 != "")
                    m[trimmed1] = trimmed2;
            }
            else
                m[f1] = f2;
        }
        return m;
    }

    /** Create comma separated string from vector of strings. */
    QString stringVectorToQString(const std::vector<QString> &v, char sep, bool escape_values)
    {
        return containerToQString(v,sep, escape_values);
    }


    /** Split a comma separated string into a vector of strings. */
    std::vector<QString> stringVectorFromQString(const QString &str, char sep, bool trim_elements)
    {
        vector<QString> vec;
        auto list = str.split(sep);

        if (trim_elements) {
            for (const auto &val : list) {
                const auto trimmed = val.trimmed();
                if (trimmed != "") vec.push_back(trimmed);
            }
        }
        else {
            for (const auto &val : list) {
                vec.push_back(val);
            }
        }
        return vec;
    }


    /** Create comma separated string from set of strings. */
    QString stringSetToQString(const std::set<QString> &set, char sep, bool escape_values)
    {
        return containerToQString(set,sep, escape_values);
    }

    /** Split a comma separated string into a set of strings. */
    std::set<QString> stringSetFromQString(const QString &str, char sep, bool trim_elements)
    {
        set<QString> set;
        auto list = str.split(sep);

        if (trim_elements) {
            for (const auto &val : list) {
                const auto trimmed = val.trimmed();
                if (trimmed != "") set.insert(trimmed);;
            }
        }
        else {
            for (const auto &val : list)
                set.insert(val);
        }
        return set;
    }




} // namespace evbase
