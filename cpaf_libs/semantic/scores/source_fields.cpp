#include <iostream>
#include <unicode/cpaf_u16string_utils.h>
#include "source_fields.h"
#include <semantic/classifier/fragments_classifier.h>

using namespace cpaf::unicode::osops;
using namespace cpaf::unicode;
using namespace std;
namespace cu = cpaf::unicode;


namespace cpaf::semantic
{

/** Add field and tokenize it. */
void source_fields::add(u16string&& field_name, const u16string& str)
{
    source_field& field = fields_map_[field_name];
    if (convert_to_lowercase_on_add_)   { field.str_ = cu::to_lower_copy(str); }
    else                                { field.str_ = str; }

    field.field_name_ = std::move(field_name);
    token_parser tk_parser(field.str_);
    tk_parser.parse(field.tokens_);
}

/** Add field and tokenize it. */
void source_fields::add(const u16string& field_name, const u16string& str)
{
    source_field& field = fields_map_[field_name];
    if (convert_to_lowercase_on_add_)   { field.str_ = cu::to_lower_copy(str); }
    else                                { field.str_ = str; }
    field.field_name_ = field_name;
    token_parser tk_parser(field.str_);
    tk_parser.parse(field.tokens_);
}

void source_fields::add(const std::string& field_name, const std::string& str)
{
    add(cpaf::unicode::to_u16string(field_name), cpaf::unicode::to_u16string(str));
}

const token::vector& source_fields::tokens(const u16string& field_name) const
{
    return fields_map_[field_name].tokens_;
}

std::u16string source_fields::dbg_u16string() const
{
    std::u16string s;
    s.append(scores().extended_debug_u16string());
    return s;
}

string source_fields::dbg_string() const
{
    return to_string(dbg_u16string());
}

void source_fields::dbg_print() const
{
    std::cerr << dbg_u16string() << "\n";

}

bool source_fields::convert_to_lowercase_on_add() const
{
    return convert_to_lowercase_on_add_;
}

void source_fields::convert_to_lowercase_on_add(bool convert_to_lowercase_on_add)
{
    convert_to_lowercase_on_add_ = convert_to_lowercase_on_add;
}

} //end namespace cpaf::semantic
