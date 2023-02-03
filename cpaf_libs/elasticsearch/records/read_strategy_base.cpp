#include "read_strategy_base.h"

#include <utf8/checked.h>

//#include <boost/filesystem.hpp>
//#include <filesystem/cpaf_file_directory_functions.h>
//#include <thehive/import/file_importer.h>

//using namespace std;
//using json = nlohmann::json;
////namespace cfs = cpaf::filesystem;
//namespace fs = boost::filesystem;

namespace cpaf::elasticsearch
{

read_strategy_base::read_strategy_base()
{
    record_string_data_.reserve(4096);
    record_string_data_clean_.reserve(4096);
}

const std::string& read_strategy_base::record_string_data() const
{
    if (!record_string_data_clean_.empty()) {
        return record_string_data_clean_;
    }
    return record_string_data_;
}


void read_strategy_base::read_string_record(std::istream& is)
{
    if (remove_illegal_chars()) {
        record_string_data_clean_ = "";
    }
    _do_read_string_record(is);
    if (remove_illegal_chars()) {
        record_string_data_clean_ = utf8::convenience::replace_invalid_json_copy_cautious(record_string_data_, 0x20);
    }
}

// ---------------------------------
// --- PRIVATE: Member functions ---
// ---------------------------------


} //end namespace cpaf::elasticsearch
