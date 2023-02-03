#ifndef CPAF_ES_READ_STRATEGY_BASE_H
#define CPAF_ES_READ_STRATEGY_BASE_H

#include <string>
#include <istream>

namespace cpaf::elasticsearch
{
class read_strategy_base
{
public:

    read_strategy_base();

    virtual ~read_strategy_base() = default;
    const std::string&  record_string_data() const;

    bool                is_stream_valid             (std::istream& is) { return _do_is_stream_valid(is); }
    void                read_string_record          (std::istream& is);

    int                 stream_max_days_old         () const { return stream_max_days_old_; }
    bool                remove_illegal_chars        () const { return remove_illegal_chars_;}
    bool                remove_long_lines           () const { return remove_long_lines_;}
    std::size_t         remove_long_lines_max_len   () const { return remove_long_lines_max_len_;}

    void                stream_max_days_old_set     (int max_days)              { stream_max_days_old_ = max_days; }
    void                remove_illegal_chars_set    (bool do_it)                { remove_illegal_chars_ = do_it;}
    void                remove_long_lines_set       (bool do_it)                { remove_long_lines_ = do_it;}
    void                remove_long_lines_max_len_set(std::size_t max_len)      { remove_long_lines_max_len_ = max_len;}
    void                project_name                (const std::string& name)   { project_name_ = name; }
    const std::string&  project_name                () const                    { return project_name_; }

protected:
    std::string record_string_data_;
    std::string record_string_data_clean_;
    std::size_t remove_long_lines_max_len_ = 500;

private:
    virtual bool _do_is_stream_valid (std::istream& /*is*/) { return true; }
    virtual void _do_read_string_record (std::istream& is) = 0;

    std::string project_name_;
    int         stream_max_days_old_   =  -1;  // Set to a number of days greater than -1 to mark that you do not want to read from a stream with too old data.

    bool        remove_illegal_chars_  = true;
    bool        remove_long_lines_     = true;
};

} //end namespace cpaf::elasticsearch

#endif //CPAF_ES_READ_STRATEGY_BASE_H
