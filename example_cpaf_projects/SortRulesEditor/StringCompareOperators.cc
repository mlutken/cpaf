#include <evbase/StringCompareOperators.h>


namespace {
    QRegExp wholeWordsMatch(const QString s, Qt::CaseSensitivity case_sensitivity)
    {
        return QRegExp(QString::fromUtf8("\\b") + s + QString::fromUtf8("\\b"), case_sensitivity);
    }
}

namespace evbase {


const std::map<QString, StringCompareOperators::Op> StringCompareOperators::m_stringToOp =
{
    {"equals", StringCompareOperators::Op::Equals},
    {"contains", StringCompareOperators::Op::Contains},
    {"startswith", StringCompareOperators::Op::StartsWith},
    {"endswith", StringCompareOperators::Op::EndsWith},
    {"regexmatch", StringCompareOperators::Op::RegexMatch},
    {"alwaystrue", StringCompareOperators::Op::AlwaysTrue},
    {"alwaysfalse", StringCompareOperators::Op::AlwaysFalse},
    {"noop", StringCompareOperators::Op::NoOp}
};


const std::map<StringCompareOperators::Op, QString> StringCompareOperators::m_opToString =
{
    {StringCompareOperators::Op::Equals, "Equals"},
    {StringCompareOperators::Op::Contains, "Contains"},
    {StringCompareOperators::Op::StartsWith, "StartsWith"},
    {StringCompareOperators::Op::EndsWith, "EndWith"},
    {StringCompareOperators::Op::RegexMatch, "RegexMatch"},
    {StringCompareOperators::Op::AlwaysTrue, "AlwaysTrue"},
    {StringCompareOperators::Op::AlwaysFalse, "AlwaysFalse"},
    {StringCompareOperators::Op::NoOp, "NoOp"}
};

StringCompareOperators::Op StringCompareOperators::stringToOp(const QString &oper)
{
    auto it_pair = m_stringToOp.find(oper.toLower());
    if (it_pair == m_stringToOp.end()) return Op::NoOp;
    return it_pair->second;
}

QString StringCompareOperators::opToString(StringCompareOperators::Op oper)
{
    auto it_pair = m_opToString.find(oper);
    if (it_pair == m_opToString.end()) return "NoOp";
    return it_pair->second;
}

StringCompareOperators::Match StringCompareOperators::stringToMatch(const QString &match)
{
    if ( match.toLower() == "wholewords")
        return Match::WholeWords;
    else
        return Match::SubString;
}

QString StringCompareOperators::matchToString(StringCompareOperators::Match match)
{
    if ( match == Match::WholeWords)
        return "WholeWords";
    else
        return "SubString";
}

Qt::CaseSensitivity StringCompareOperators::stringToCaseSensitivity(const QString &case_sensitivity)
{
    if ( case_sensitivity.toLower() == "caseinsensitive")
        return Qt::CaseInsensitive;
    else
        return Qt::CaseSensitive;
}

QString StringCompareOperators::caseSensitivityToString(Qt::CaseSensitivity case_sensitivity)
{
    if ( case_sensitivity == Qt::CaseInsensitive)
        return "CaseInsensitive";
    else
        return "CaseSensitive";
}

bool StringCompareOperators::op(StringCompareOperators::Op oper, const QString &search_in,
                                const QString &subject, StringCompareOperators::Match match_type,
                                Qt::CaseSensitivity case_sensitivity)
{
    switch (oper) {
    case Op::Equals:
        return equals(search_in, subject, match_type, case_sensitivity);
        break;
    case Op::Contains:
        return contains(search_in, subject, match_type, case_sensitivity);
        break;
    case Op::StartsWith:
        return startsWith(search_in, subject, match_type, case_sensitivity);
        break;
    case Op::EndsWith:
        return endsWith(search_in, subject, match_type, case_sensitivity);
        break;
    case Op::RegexMatch:
        return regexMatch(search_in, subject, match_type, case_sensitivity);
        break;
    case Op::AlwaysTrue:
        return alwaysTrue(search_in, subject, match_type, case_sensitivity);
        break;
    case Op::AlwaysFalse:
        return alwaysFalse(search_in, subject, match_type, case_sensitivity);
        break;
    default:
        break;
    }
    return noOp(search_in, subject, match_type, case_sensitivity);
}

StringCompareOperators::OpFn StringCompareOperators::opFunction(StringCompareOperators::Op oper)
{
    return  [oper] (const QString &search_in, const QString &subject,Match match_type, Qt::CaseSensitivity case_sensitivity)
                    { return StringCompareOperators::op(oper, search_in, subject, match_type, case_sensitivity); };
//    return std::bind(&StringCompareOperators::op, oper, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

bool StringCompareOperators::equals(const QString &search_in, const QString &subject,
                                    StringCompareOperators::Match, Qt::CaseSensitivity case_sensitivity)
{
    const int cmp = search_in.compare(subject, case_sensitivity);
    return (cmp == 0);
}

bool StringCompareOperators::equals(const QString &search_in, const QString &subject,
                                    Qt::CaseSensitivity case_sensitivity)
{
    const int cmp = search_in.compare(subject, case_sensitivity);
    return (cmp == 0);
}

bool StringCompareOperators::contains(const QString &search_in, const QString &subject,
                                      Match match_type, Qt::CaseSensitivity case_sensitivity)
{
    if (match_type == Match::SubString) {
        return search_in.contains(subject, case_sensitivity);
    }
    else {
        const int pos = search_in.indexOf (wholeWordsMatch(subject, case_sensitivity));
        return (pos != -1);
    }
}

bool StringCompareOperators::startsWith(const QString &search_in, const QString &subject,
                                        StringCompareOperators::Match match_type, Qt::CaseSensitivity case_sensitivity)
{
    if (match_type == Match::SubString) {
        return search_in.startsWith(subject, case_sensitivity);
    }
    else {
        const int pos = search_in.indexOf (wholeWordsMatch(subject, case_sensitivity));
        return (pos == 0);
    }
}

bool StringCompareOperators::endsWith(const QString &search_in, const QString &subject,
                                      StringCompareOperators::Match match_type, Qt::CaseSensitivity case_sensitivity)
{
    if (match_type == Match::SubString) {
        return search_in.endsWith(subject, case_sensitivity);
    }
    else {
        const int pos = search_in.lastIndexOf(wholeWordsMatch(subject, case_sensitivity));
        return (pos + subject.length()) == search_in.length();
    }
}

bool StringCompareOperators::regexMatch(const QString &search_in, const QString &subject,
                                        StringCompareOperators::Match, Qt::CaseSensitivity case_sensitivity)
{
    const int pos = search_in.indexOf (QRegExp(subject, case_sensitivity));
    return (pos != -1);
}

bool StringCompareOperators::regexMatch(const QString &search_in, const QString &subject,
                                        Qt::CaseSensitivity case_sensitivity)
{
    const int pos = search_in.indexOf (QRegExp(subject, case_sensitivity));
    return (pos != -1);
}

bool StringCompareOperators::alwaysTrue(const QString &, const QString &, StringCompareOperators::Match, Qt::CaseSensitivity)
{
    return true;
}

bool StringCompareOperators::alwaysFalse(const QString &, const QString &, StringCompareOperators::Match, Qt::CaseSensitivity)
{
    return false;
}

bool StringCompareOperators::noOp(const QString &, const QString &, StringCompareOperators::Match, Qt::CaseSensitivity)
{
    return false;
}


} // namespace evbase
