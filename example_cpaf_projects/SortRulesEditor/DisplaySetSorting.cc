#include <vdbclient/DisplaySetSorting.h>

#include <limits>
#include <QScriptEngine>
#include <evbase/SerializationUtils.h>

using namespace std;
using namespace evbase;

namespace vdbclient {


const std::map<QString, DisplaySetSortOpBase::SortValueType> DisplaySetSortOpBase::m_stringToSortValueType =
{
    {"int", DisplaySetSortOpBase::SortValueType::Int},
    {"string", DisplaySetSortOpBase::SortValueType::String}
};

const std::map<DisplaySetSortOpBase::SortValueType, QString> DisplaySetSortOpBase::m_sortValueTypeToString =
{
    {DisplaySetSortOpBase::SortValueType::Int, "Int"},
    {DisplaySetSortOpBase::SortValueType::String, "String"}
};

DisplaySetSortOpBase::SortValueType DisplaySetSortOpBase::stringToSortValueType(const QString &sort_value_type)
{
    auto it_pair = m_stringToSortValueType.find(sort_value_type.toLower());
    if (it_pair == m_stringToSortValueType.end()) return SortValueType::String;
    return it_pair->second;
}

QString DisplaySetSortOpBase::sortValueTypeToString(DisplaySetSortOpBase::SortValueType sort_value_type)
{
    auto it_pair = m_sortValueTypeToString.find(sort_value_type);
    if (it_pair == m_sortValueTypeToString.end()) return "String";
    return it_pair->second;
}


QString DisplaySetSortOpBase::sortDirectionToString(const DisplaySetSortOpBase::SortDirection &sort_direction)
{
    if ( sort_direction == SortDirection::Ascending)
        return "Ascending";
    else
        return "Descending";
}

DisplaySetSortOpBase::SortDirection DisplaySetSortOpBase::stringToSortDirection(const QString &sort_direction)
{
    if ( sort_direction.toLower() == "ascending")
        return SortDirection::Ascending;
    else
        return SortDirection::Descending;
}


void DisplaySetSortOpBase::setSortDirection(const QString &sort_direction)
{
    m_sort_direction = stringToSortDirection(sort_direction);
}


bool operator<(const DisplaySetSortKeys &lhs, const DisplaySetSortKeys &rhs)
{
    const size_t size = lhs.size();
    if (rhs.size() != size) return false;
    for ( size_t i = 0; i < size; ++i) {
        const DisplaySetSortOpBase &l = *(lhs[i]);
        const DisplaySetSortOpBase &r = *(rhs[i]);
        if (l.less(r)) return true;
        else if (r.less(l)) return false;
    }
    return false;
}

// --------------------------
// --- DisplaySetSortRule ---
// --------------------------

const DisplaySetSortRule::StringVec DisplaySetSortRule::m_allSortRuleFields
{
    "sort_type", "sort_value_type", "sort_direction", "compare_operator", "match_type", "case_sensitivity", "field_name"
};

DisplaySetSortRule::SortType DisplaySetSortRule::stringToSortType(const QString &sort_type)
{
    if ( sort_type.toLower() == "normalsort")
        return SortType::NormalSort;
    else
        return SortType::CustomSortOrder;

}

QString DisplaySetSortRule::sortTypeToString(DisplaySetSortRule::SortType sort_type)
{
    if ( sort_type == SortType::NormalSort)
        return "NormalSort";
    else
        return "CustomSortOrder";
}

void DisplaySetSortRule::fromScriptValue(const QScriptValue &sort_rule_obj)
{
    m_sort_type = stringToSortType(sort_rule_obj.property("sort_type").toString());
    m_sort_value_type = DisplaySetSortOpBase::stringToSortValueType(sort_rule_obj.property("sort_value_type").toString());
    m_sort_direction = DisplaySetSortOpBase::stringToSortDirection(sort_rule_obj.property("sort_direction").toString());
    m_compare_operator = StringCompareOperators::stringToOp(sort_rule_obj.property("compare_operator").toString());
    m_match_type = StringCompareOperators::stringToMatch(sort_rule_obj.property("match_type").toString());
    m_case_sensitivity = StringCompareOperators::stringToCaseSensitivity(sort_rule_obj.property("case_sensitivity").toString());
    m_field_name = sort_rule_obj.property("field_name").toString();

    m_values_order.clear();
    QScriptValue array_values_order = sort_rule_obj.property("values_order");
    const int length = array_values_order.property("length").toInteger();
    for ( int i = 0; i < length; ++i) {
        m_values_order.push_back(array_values_order.property(i).toString());
    }
}

QScriptValue DisplaySetSortRule::toScriptValue(QScriptEngine *engine) const
{
    QScriptValue obj = engine->newObject();
    obj.setProperty("sort_type", sortTypeToString(m_sort_type));
    obj.setProperty("sort_value_type", DisplaySetSortOpBase::sortValueTypeToString(m_sort_value_type));
    obj.setProperty("sort_direction", DisplaySetSortOpBase::sortDirectionToString(m_sort_direction));
    obj.setProperty("compare_operator", StringCompareOperators::opToString(m_compare_operator));
    obj.setProperty("match_type", StringCompareOperators::matchToString(m_match_type));
    obj.setProperty("case_sensitivity", StringCompareOperators::caseSensitivityToString(m_case_sensitivity));
    obj.setProperty("field_name", m_field_name);

    const unsigned array_size = m_values_order.size();
    QScriptValue values_order_array = engine->newArray(array_size);
    for ( size_t i = 0; i < array_size; ++i) {
        values_order_array.setProperty(i, m_values_order[i]);
    }
    obj.setProperty("values_order", values_order_array);
    return obj;
}

bool DisplaySetSortRule::stringCompareOp(const QString &search_in, const QString &subject) const
{
    return StringCompareOperators::op(m_compare_operator, search_in, subject, m_match_type, m_case_sensitivity);
}

DisplaySetSortRule::SortType DisplaySetSortRule::sortType() const
{
    return m_sort_type;
}

void DisplaySetSortRule::setSortType(const SortType &sort_type)
{
    m_sort_type = sort_type;
}

DisplaySetSortRule::SortValueType DisplaySetSortRule::sortValueType() const
{
    return m_sort_value_type;
}

void DisplaySetSortRule::setSortValueType(const SortValueType &sort_value_type)
{
    m_sort_value_type = sort_value_type;
}

DisplaySetSortRule::SortDirection DisplaySetSortRule::sortDirection() const
{
    return m_sort_direction;
}

void DisplaySetSortRule::setSortDirection(const SortDirection &sort_direction)
{
    m_sort_direction = sort_direction;
}

DisplaySetSortRule::Op DisplaySetSortRule::compareOperator() const
{
    return m_compare_operator;
}

void DisplaySetSortRule::setCompareOperator(const Op &compare_type)
{
    m_compare_operator = compare_type;
}

DisplaySetSortRule::Match DisplaySetSortRule::matchType() const
{
    return m_match_type;
}

void DisplaySetSortRule::setMatchType(const Match &match_type)
{
    m_match_type = match_type;
}

QString DisplaySetSortRule::fieldName() const
{
    return m_field_name;
}

void DisplaySetSortRule::setFieldName(const QString &field_name)
{
    m_field_name = field_name;
}

Qt::CaseSensitivity DisplaySetSortRule::caseSensitivity() const
{
    return m_case_sensitivity;
}

void DisplaySetSortRule::setCaseSensitivity(const Qt::CaseSensitivity &case_sensitivity)
{
    m_case_sensitivity = case_sensitivity;
}

const std::vector<QString> &DisplaySetSortRule::valuesOrder() const
{
    return m_values_order;
}

void DisplaySetSortRule::setValuesOrder(const std::vector<QString> &values_order)
{
    m_values_order = values_order;
}

void DisplaySetSortRule::setValuesOrder(const QString &values_order_comma_separated)
{
    m_values_order = stringVectorFromQString(values_order_comma_separated,  ',', true);
}

QString DisplaySetSortRule::dbgString() const
{
    QString s;
    s += "sort_type: " + sortTypeToString(m_sort_type) + "\n";
    s += "sort_value_type: " + DisplaySetSortOpBase::sortValueTypeToString(m_sort_value_type) + "\n";
    s += "sort_direction: " + DisplaySetSortOpBase::sortDirectionToString(m_sort_direction) + "\n";
    s += "compare_operator: " + StringCompareOperators::opToString(m_compare_operator) + "\n";
    s += "match_type: " + StringCompareOperators::matchToString(m_match_type) + "\n";
    s += "case_sensitivity: " + StringCompareOperators::caseSensitivityToString(m_case_sensitivity) + "\n";
    s += "field_name: " + m_field_name + "\n";

    s += "values_order: [ ";
    for ( size_t i = 0; i < m_values_order.size(); ++i) {
        if (i>0)
            s += ", ";
        s += m_values_order[i];
    }
    s += "]\n";
    return s;
}


DisplaySetSortRules sortRulesFromScriptValue(const QScriptValue &sort_rules_array)
{
    DisplaySetSortRules sort_rules;
    const int length = sort_rules_array.property("length").toInteger();

    for ( int i = 0; i < length; ++i) {
        QScriptValue sort_rule_obj = sort_rules_array.property(i);
        DisplaySetSortRule sort_rule;
        sort_rule.fromScriptValue(sort_rule_obj);
        sort_rules.push_back(sort_rule);
    }
    return sort_rules;
}


QScriptValue sortRulesToScriptValue(QScriptEngine *engine, const DisplaySetSortRules &sort_rules)
{
    QScriptValue sort_rules_array = engine->newArray(sort_rules.size());
    for ( size_t i = 0; i < sort_rules.size(); ++i) {
        QScriptValue sort_rule_obj = sort_rules[i].toScriptValue(engine);
        sort_rules_array.setProperty(i, sort_rule_obj);
    }
    return sort_rules_array;
}


// ---------------------------------
// --- DisplaySetSortRulesParser ---
// ---------------------------------


DisplaySetSortRulesParser::DisplaySetSortRulesParser(const vdbclient::SynonymsManager &synonyms_manager)
    : m_synonyms_manager(synonyms_manager)
{

}

std::shared_ptr<DisplaySetSortOpBase> DisplaySetSortRulesParser::create(const DisplaySetSortRule &rule)
{
    if (DisplaySetSortRule::SortType::NormalSort == rule.sortType())
        return createNormal(rule);
    else
        return createCustom(rule);
}

DisplaySetSortKeys DisplaySetSortRulesParser::create(const DisplaySetSortRules &rules)
{
    DisplaySetSortKeys sort_criteria;
    for (auto rule : rules) {
        sort_criteria.push_back(create(rule));
    }
    return sort_criteria;
}

std::shared_ptr<DisplaySetSortOpBase> DisplaySetSortRulesParser::createNormal(const DisplaySetSortRule &rule)
{
    std::shared_ptr<StringSortOp> sort_op (new StringSortOp);
    sort_op->setValue(m_get_value_fn(rule.fieldName()));
    sort_op->setSortDirection(rule.sortDirection());
    return std::move(sort_op);
}

std::shared_ptr<DisplaySetSortOpBase> DisplaySetSortRulesParser::createCustom(const DisplaySetSortRule &rule)
{
    std::shared_ptr<IntSortOp> sort_op (new IntSortOp);
    sort_op->setSortDirection(DisplaySetSortOpBase::SortDirection::Descending);

    int val = std::numeric_limits<int>::max();
    const QString search_in_val = m_get_value_fn(rule.fieldName());
    const std::vector<QString> &values_order = rule.valuesOrder();
    const size_t size = values_order.size();
    for ( size_t i = 0; i < size; ++i) {
        const QString &custom_value = values_order[i];
        if (SynonymsManager::isSynonymString(custom_value)) {
            QString synonym_list_name = SynonymsManager::synonymStringToSynonym(custom_value);
            for ( const QString &synonym_val : m_synonyms_manager.synonymsForName(synonym_list_name)) {
                if (rule.stringCompareOp(search_in_val, synonym_val)){
                    val = i;
                    break;
                }
            }
        }
        else if (rule.stringCompareOp(search_in_val, custom_value)){
            val = i;
            break;
        }
    }
    sort_op->setValue(val);
    return sort_op;
}


void DisplaySetSortRulesParser::setGetValueFn(const GetValueFn &get_value_fn)
{
    m_get_value_fn = get_value_fn;
}


} // END namespace vdbclient
