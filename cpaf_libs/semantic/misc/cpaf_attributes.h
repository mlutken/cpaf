#ifndef CPAF_ATTRIBUTES_H
#define CPAF_ATTRIBUTES_H

#include <memory>
#include <bitset>
#include <map>
#include <vector>
#include <unordered_map>
#include <string>


namespace cpaf::semantic
{
class fragment_node;

// -----------------
// --- attribute ---
// -----------------

class attribute
{
public:
    using vector = std::vector<std::unique_ptr<attribute>>;

    virtual ~attribute() = default;
    std::u16string  class_name  (const fragment_node* node) const { return do_string_value(node); }
    std::u16string  string_value(const fragment_node* node) const { return do_string_value(node); }
    float           number_value(const fragment_node* node) const { return do_number_value(node); }

    // ----------------------------------
    // --- PUBLIC: Setter and getters ---
    // ----------------------------------
    const std::u16string&   type        () const                            { return type_; }
    void                    type        (const std::u16string& type)        { type_ = type; }
    const std::u16string&   parent_type () const                            { return parent_type_; }
    void                    parent_type (const std::u16string& parent_type) { parent_type_ = parent_type; }
    attribute*              parent_ptr  () const                            { return parent_ptr_; }
    void                    parent_ptr  (attribute* parent_ptr)             { parent_ptr_ = parent_ptr; }

private:
    virtual std::u16string  do_class_name   () const = 0;
    virtual std::u16string  do_string_value (const fragment_node* node) const = 0;
    virtual float           do_number_value (const fragment_node* node) const = 0;

    std::u16string  type_;
    std::u16string  parent_type_;
    attribute*      parent_ptr_ = nullptr;

};

// --------------------------
// --- attribute_standard ---
// --------------------------

class attribute_standard : public attribute
{
public:
    using attribute::attribute;
    // ----------------------------------
    // --- PUBLIC: Setter and getters ---
    // ----------------------------------
    const std::u16string&   value       () const                            { return value_; }
    void                    value       (const std::u16string& value)       { value_ = value;}

    std::u16string  do_class_name   () const override { return u"standard"; }
    std::u16string  do_string_value (const fragment_node* node) const override;
    float           do_number_value (const fragment_node* node) const override;
private:
    std::u16string  value_;
};

class attribute_unit : public attribute
{
public:
    using attribute::attribute;
    // ----------------------------------
    // --- PUBLIC: Setter and getters ---
    // ----------------------------------
    const std::u16string&   value       () const                            { return value_; }
    void                    value       (const std::u16string& value)       { value_ = value;}
    float                   factor      () const                            { return factor_; }
    void                    factor      (const float factor)                { factor_ = factor;}

    std::u16string  do_class_name   () const override { return u"standard"; }
    std::u16string  do_string_value (const fragment_node* node) const override;
    float           do_number_value (const fragment_node* node) const override;
private:
    std::u16string  value_;
    float           factor_ = 1;
};

// -------------------------
// --- attribute_factory ---
// -------------------------

class attribute_factory {
public:
    attribute_factory() = default;
    std::unique_ptr<attribute>  create(const std::u16string& class_name);
};

// ---------------------------
// --- attributes_mappings ---
// ---------------------------

class attributes_mappings {
public:
    attributes_mappings();

    size_t          push_back   (const std::u16string& attribute_name);
    size_t          name_to_pos (const std::u16string& attribute_name) const ;
    std::u16string  pos_to_name (size_t pos) const;

private:
    using pos_to_name_vec = std::vector<std::u16string>;
    using name_to_pos_map = std::unordered_map<std::u16string, size_t>;

    pos_to_name_vec pos_to_name_;
    name_to_pos_map name_to_pos_;
};

// ------------------
// --- attributes ---
// ------------------

class attributes_set {
public:
    using bitset = std::bitset<256>;
    using reference = bitset::reference;

    void set        (const attributes_mappings& mappings, const std::u16string& attribute_name, bool value= true);
    void reset      (const attributes_mappings& mappings, const std::u16string& attribute_name);
    bool test       (const attributes_mappings& mappings, const std::u16string& attribute_name);


    void set                    (size_t pos, bool value= true)  { attributes_.set(pos, value);  }
    void reset                  (size_t pos)                    { attributes_.reset(pos);       }
    constexpr bool  operator[]  (size_t pos) const              { return attributes_[pos];      }
    reference       operator[]  (size_t pos)                    { return attributes_[pos];      }
    bool test                   (size_t pos) const              { return attributes_.test(pos); }

private:
    bitset attributes_;
};



} //end namespace cpaf::semantic


#endif //CPAF_ATTRIBUTES_H

