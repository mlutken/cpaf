#ifndef CLASSIFICATION_DATA_READER_BASE_H
#define CLASSIFICATION_DATA_READER_BASE_H

#include <string>
#include <map>
#include <boost/filesystem.hpp>
#include <utils/cpaf_json_utils.h>
#include <unicode/cpaf_u16string_utils.h>

namespace cpaf::semantic
{

class classification_fragment_reader_base
{
public:
    /** The weights map has source fields as keys and the weight for the source field
        as the values */
    using weights_map = std::map<std::string, int>;
    static const weights_map default_weights;

    classification_fragment_reader_base();
    virtual ~classification_fragment_reader_base() = default;
    explicit classification_fragment_reader_base(const std::string& resource_path_or_data);
    explicit classification_fragment_reader_base(const boost::filesystem::path& resource_path_or_data);

    const weights_map&  global_weights  () const { return global_weights_per_source_field_; }
    void                global_weights  (const weights_map& weights) { global_weights_per_source_field_ = weights; should_read_data_ = true; }
    std::string         language_code   () const                    { return language_code_;}
    void                language_code   (const std::string& lc)     { language_code_ = lc;  }

    const std::string&  resource        () const;
    void                resource        (const std::string& resource_path_or_data);
    void                resource        (const boost::filesystem::path& path);
    void                read_data       ();
    void                begin_iterating ();
    bool                iterating_done  ()      { return do_iterating_done();   }
    void                next_fragment   ()      { do_next_fragment();           }

    std::string         custom_field    (const std::string& key, const std::string& default_value = "") const     { return do_custom_field(key, default_value);         }
    std::u16string      u16_custom_field(const std::string& key, const std::string& default_value = "") const     { return cpaf::unicode::to_u16string(custom_field(key, default_value)); }
    std::string         fragment        () const    { return do_fragment();         }
    std::u16string      u16_fragment    () const    { return cpaf::unicode::to_u16string(fragment()); }
    std::string         type            () const    { return do_type();         }
    std::u16string      u16_type        () const    { return cpaf::unicode::to_u16string(type()); }
    std::string         value           () const    { return do_value();         }
    std::u16string      u16_value       () const    { return cpaf::unicode::to_u16string(value()); }
    std::string         source_field    () const    { return do_source_field();         }
    std::u16string      u16_source_field() const    { return cpaf::unicode::to_u16string(source_field()); }
    int                 weight          () const    { return do_weight();         }

protected:
    void                should_read_data_set(bool should_read) { should_read_data_ = should_read; }

private:
    virtual void        do_read_data        () = 0;
    virtual void        do_begin_iterating  () = 0;
    virtual bool        do_iterating_done   () = 0;
    virtual void        do_next_fragment    () = 0;
    virtual std::string do_custom_field     (const std::string& key, const std::string& default_value) const = 0;
    virtual std::string do_fragment         () const = 0;
    virtual std::string do_type             () const = 0;
    virtual std::string do_value            () const = 0;
    virtual std::string do_source_field     () const = 0;
    virtual int         do_weight           () const = 0;

    weights_map     global_weights_per_source_field_;
    std::string     language_code_;
    std::string     resource_;  // Path to resource or actual resource data
    bool            should_read_data_ = true;
};

// --------------------------------
// --- Reader utility functions ---
// --------------------------------
std::string     parse_fragment_weights      (std::map<std::string, int>& weights_map, const std::string& fragment);
nlohmann::json  fragment_to_json            (const std::string& raw_fragment);

} //end namespace cpaf::semantic

#endif // CLASSIFICATION_DATA_READER_BASE_H
