#ifndef JSON_CLASSIFICATION_DATA_READER_H
#define JSON_CLASSIFICATION_DATA_READER_H

#include <utils/cpaf_json_utils.h>
#include <semantic/readers/classification_data_reader_base.h>

namespace cpaf::semantic
{

class json_classification_data_reader : public classification_fragment_reader_base
{
public:
    explicit json_classification_data_reader(const std::string& path);
    explicit json_classification_data_reader(const boost::filesystem::path& path);

    const nlohmann::json&       json_data        () const { return json_data_; }
    const nlohmann::json&       values           () const;

    std::string                 dbg_default_weights() const;

private:
    // --- PRIVATE: Virtual functions overridden here ---
    void        do_read_data        () override;
    void        do_begin_iterating  () override;
    bool        do_iterating_done   () override;
    void        do_next_fragment    () override;
    std::string do_custom_field     (const std::string& key, const std::string& default_value) const override;
    std::string do_fragment         () const override;
    std::string do_type             () const override;
    std::string do_value            () const override;
    int         do_weight           () const override;
    std::string do_source_field     () const override;

    // --- PRIVATE: Helper functions ---
    using language_codes_vec = std::vector<std::string>;
    using language_codes_const_iterator = language_codes_vec::const_iterator;
    bool        seek_first_valid_value      ();
    bool        seek_first_valid_fragments  ();
    bool        create_fragments_iterators  ();
    void        create_weights_iterators    ();
    std::string fragment_iterator_value     () const;

    const nlohmann::json&      default_json_weights   () const;

    // --- PRIVATE: Member data ---
    weights_map                         default_fragment_weights_;  // This combines the json files default weight with the global_weights() from the base class
    weights_map                         current_fragment_weights_;  // This map contains the default_fragment_weights_ (source_field,weight) combined with any local fragment weights
    std::vector<std::string>            language_codes_;    // Add fragments from these language codes
    nlohmann::json                      json_data_;
    const nlohmann::json*               json_fragments_ptr_;
    const nlohmann::json                empty_json_;
    nlohmann::json::const_iterator      values_iterator_;
    language_codes_const_iterator       languages_iterator_;
    nlohmann::json::const_iterator      fragments_iterator_;
    nlohmann::json::const_iterator      fragments_iterator_end_;
    weights_map::const_iterator         weights_iterator_;
    weights_map::const_iterator         weights_iterator_end_;
    std::string                         fragment_;  //Fragment to return in case the "raw" fragment has source fields and weight specified and thus needs parsing. For example "[breadcrumbs_text:100, item_title:200] cykeltaske"

};

} //end namespace cpaf::semantic

#endif // JSON_CLASSIFICATION_DATA_READER_H
