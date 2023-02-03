#ifndef EVBASE_STRINGCOMPAREOPERATORS_H
#define EVBASE_STRINGCOMPAREOPERATORS_H

#include <functional>
#include <unordered_map>
#include <map>
#include <string>
#include <QString>
#include <QRegExp>

namespace evbase {

class StringCompareOperators
{
public:
    enum class Op { Equals=0, Contains, StartsWith, EndsWith, RegexMatch, AlwaysTrue, AlwaysFalse, NoOp };
    enum Match { WholeWords, SubString };
    using OpFn = std::function<bool (const QString &, const QString &, Match, Qt::CaseSensitivity)>;
    using Op2Fn = std::function<bool (const QString &, const QString &)>;

    static const std::map<QString, Op> m_stringToOp;
    static const std::map<Op, QString> m_opToString;
    static Op stringToOp (const QString & oper);
    static QString opToString (Op oper);
    static Match stringToMatch (const QString &match);
    static QString matchToString (Match match);
    static Qt::CaseSensitivity stringToCaseSensitivity (const QString &case_sensitivity);
    static QString caseSensitivityToString (Qt::CaseSensitivity case_sensitivity);

    static bool op( Op oper, const QString &search_in, const QString &subject,
                    Match match_type, Qt::CaseSensitivity case_sensitivity);


    static OpFn opFunction(Op oper);

    static bool equals(const QString &search_in, const QString &subject, Match, Qt::CaseSensitivity case_sensitivity);
    static bool equals(const QString &search_in, const QString &subject, Qt::CaseSensitivity case_sensitivity);
    static bool contains(const QString &search_in, const QString &subject,
                         Match match_type, Qt::CaseSensitivity case_sensitivity);
    static bool startsWith(const QString &search_in, const QString &subject,
                           Match match_type, Qt::CaseSensitivity case_sensitivity);
    static bool endsWith(const QString &search_in, const QString &subject,
                         Match match_type, Qt::CaseSensitivity case_sensitivity);
    static bool regexMatch(const QString &search_in, const QString &subject,
                           Match, Qt::CaseSensitivity case_sensitivity);
    static bool regexMatch(const QString &search_in, const QString &subject,
                           Qt::CaseSensitivity case_sensitivity);
    static bool alwaysTrue(const QString &, const QString &,
                           Match, Qt::CaseSensitivity);
    static bool alwaysFalse(const QString &, const QString &,
                            Match, Qt::CaseSensitivity);
    static bool noOp(const QString &, const QString &,
                            Match, Qt::CaseSensitivity);

private:

};

} // namespace evbase

#endif // EVBASE_STRINGCOMPAREOPERATORS_H
