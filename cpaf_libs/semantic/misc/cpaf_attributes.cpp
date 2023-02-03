#include <unicode/cpaf_u16string_utils.h>
#include <semantic/fragments/fragment_node.h>
#include "cpaf_attributes.h"



namespace cpaf::semantic
{

// -----------------
// --- attribute ---
// -----------------

// --------------------------
// --- attribute_standard ---
// --------------------------

std::u16string attribute_standard::do_string_value(const fragment_node* /*node*/) const
{
    return value_;
}

float attribute_standard::do_number_value(const fragment_node* /*node*/) const
{
    return 0;
}


// -------------------------
// --- attribute_factory ---
// -------------------------
std::unique_ptr<attribute> attribute_factory::create(const std::u16string& class_name)
{
    if (class_name == u"unit") {
        return std::make_unique<attribute_unit>();
    }

    return std::make_unique<attribute_standard>();
}

// ---------------------------
// --- attributes_mappings ---
// ---------------------------

attributes_mappings::attributes_mappings()
{
    push_back(u"_INVALID_");
}

size_t attributes_mappings::push_back(const std::u16string& attribute_name)
{
    pos_to_name_.push_back(attribute_name);
    const auto pos = pos_to_name_.size() -1;
    name_to_pos_[attribute_name] = pos;
    return pos;
}

size_t attributes_mappings::name_to_pos(const std::u16string& attribute_name) const
{
    const auto it = name_to_pos_.find(attribute_name);
    if (it != name_to_pos_.end()) {
        return it->second;
    }
    return 0;
}

std::u16string attributes_mappings::pos_to_name(size_t pos) const
{
    if ( 0 < pos && pos < pos_to_name_.size()) {
        return pos_to_name_[pos];
    }
    return u"";
}

void attributes_set::set(const attributes_mappings& mappings, const std::u16string& attribute_name, bool value)
{
    set(mappings.name_to_pos(attribute_name), value);
}

void attributes_set::reset(const attributes_mappings& mappings, const std::u16string& attribute_name)
{
    reset(mappings.name_to_pos(attribute_name));
}

bool attributes_set::test(const attributes_mappings& mappings, const std::u16string& attribute_name)
{
    return test(mappings.name_to_pos(attribute_name));
}




} //end namespace cpaf::semantic


