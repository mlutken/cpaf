#ifndef VDBCLIENT_DISPLAYSETSORTING_H
#define VDBCLIENT_DISPLAYSETSORTING_H

#include <vector>
#include <map>
#include <memory>
#include <QString>
#include <QDateTime>
#include <QScriptValue>
#include <evbase/StringCompareOperators.h>
#include <vdbclient/SynonymsManager.h>
class QScriptEngine;

namespace vdbclient {

class DisplaySetSortOpBase
{
public:
//    enum ValueType { Unknown, Void, Int, Unsigned, Float, String, DateTime };
    enum SortValueType { Int, String };
    enum class SortDirection { Ascending, Descending };

    const static std::map<QString, SortValueType> m_stringToSortValueType;
    const static std::map<SortValueType, QString> m_sortValueTypeToString;

    static SortValueType stringToSortValueType (const QString &sort_value_type);
    static QString sortValueTypeToString (SortValueType sort_value_type);

    static QString sortDirectionToString (const SortDirection &sort_direction);
    static SortDirection stringToSortDirection (const QString &sort_direction);

    DisplaySetSortOpBase() = default;
    virtual ~DisplaySetSortOpBase(){}
    virtual bool less(const DisplaySetSortOpBase &rhs) const = 0;
    virtual SortValueType valueType() const = 0;
//    virtual void setValueFromString(const QString & val) = 0;

    SortDirection sortDirection() const
    {
        return m_sort_direction;
    }

    void setSortDirection(const SortDirection &sort_direction)
    {
        m_sort_direction = sort_direction;
    }

    void setSortDirection(const QString &sort_direction);

private:
    SortDirection m_sort_direction = SortDirection::Ascending;
};


template <typename TYPE, int VALUE_TYPE>
class DisplaySetSortOp : public DisplaySetSortOpBase
{
public:
    using DisplaySetSortType = DisplaySetSortOp<TYPE, VALUE_TYPE>;

    virtual bool less(const DisplaySetSortOpBase &rhs) const override
    {
        const DisplaySetSortType &r = static_cast<const DisplaySetSortType &>(rhs);
        if ( sortDirection() == SortDirection::Ascending)
            return !(value() < r.value());
        else
            return value() < r.value();

    }

    virtual SortValueType valueType() const override
    {
        return static_cast<DisplaySetSortOpBase::SortValueType>(VALUE_TYPE);
    }

//    virtual void setValueFromString(const QString & val)
//    {
//        QString::n
//    }


    TYPE value() const
    {
        return m_value;
    }

    void setValue(const TYPE &value)
    {
        m_value = value;
    }

private:
    TYPE m_value;
};

using IntSortOp = DisplaySetSortOp<int, DisplaySetSortOpBase::Int>;
using StringSortOp = DisplaySetSortOp<QString, DisplaySetSortOpBase::String>;

using DisplaySetSortKeys = std::vector<std::shared_ptr<DisplaySetSortOpBase>>;
bool operator<(const DisplaySetSortKeys &lhs, const DisplaySetSortKeys &rhs);

// --------------------------
// --- DisplaySetSortRule ---
// --------------------------

class DisplaySetSortRule
{
public:
    enum class SortType { NormalSort, CustomSortOrder };

    using SortValueType = DisplaySetSortOpBase::SortValueType;
    using SortDirection = DisplaySetSortOpBase::SortDirection;
    using Op = evbase::StringCompareOperators::Op;
    using Match = evbase::StringCompareOperators::Match;

    using StringVec =  std::vector<QString>;

    const static StringVec m_allSortRuleFields;
    static SortType stringToSortType (const QString &sort_type);
    static QString sortTypeToString (SortType sort_type);

    void fromScriptValue (const QScriptValue &sort_rule_obj);
    QScriptValue toScriptValue (QScriptEngine *engine) const;
    bool stringCompareOp( const QString &search_in, const QString &subject) const;

    SortType sortType() const;
    void setSortType(const SortType &sort_type);
    SortValueType sortValueType() const;
    void setSortValueType(const SortValueType &sort_value_type);
    SortDirection sortDirection() const;
    void setSortDirection(const SortDirection &sort_direction);
    Op compareOperator() const;
    void setCompareOperator(const Op &compare_type);
    Match matchType() const;
    void setMatchType(const Match &match_type);
    QString fieldName() const;
    void setFieldName(const QString &field_name);
    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(const Qt::CaseSensitivity &case_sensitivity);

    const std::vector<QString> &valuesOrder() const;
    void setValuesOrder(const std::vector<QString> &values_order);
    void setValuesOrder(const QString &values_order_comma_separated);

    QString dbgString() const;
private:
    SortType m_sort_type = SortType::NormalSort;
    SortValueType m_sort_value_type = SortValueType::String;
    SortDirection m_sort_direction = SortDirection::Descending;
    Op m_compare_operator = Op::Contains;
    Match m_match_type = Match::SubString;
    Qt::CaseSensitivity m_case_sensitivity = Qt::CaseInsensitive;
    QString m_field_name;
    std::vector<QString> m_values_order;

};


using DisplaySetSortRules = std::vector<DisplaySetSortRule>;

/** Get vector of sort rules from a script value.
 * The script value is assumed to be an array of sort objects. */
DisplaySetSortRules sortRulesFromScriptValue(const QScriptValue &sort_rules_array);

/** Convert vector of sort rules to a script value. */
QScriptValue sortRulesToScriptValue(QScriptEngine *engine, const DisplaySetSortRules &sort_rules);

// ---------------------------------
// --- DisplaySetSortRulesParser ---
// ---------------------------------

class DisplaySetSortRulesParser
{
public:
    using GetValueFn = std::function<QString (const QString &)>;
    explicit DisplaySetSortRulesParser(const vdbclient::SynonymsManager &synonyms_manager);

    std::shared_ptr<DisplaySetSortOpBase> create(const DisplaySetSortRule & rule);
    DisplaySetSortKeys create( const DisplaySetSortRules &rules);

    void setGetValueFn(const GetValueFn &get_value_fn);

private:
    std::shared_ptr<DisplaySetSortOpBase> createNormal(const DisplaySetSortRule &rule);
    std::shared_ptr<DisplaySetSortOpBase> createCustom(const DisplaySetSortRule &rule);

    const vdbclient::SynonymsManager &m_synonyms_manager;
    GetValueFn m_get_value_fn;
};


} // END namespace vdbclient


#endif // VDBCLIENT_DISPLAYSETSORTING_H
