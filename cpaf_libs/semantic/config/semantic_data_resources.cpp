#include "semantic_data_resources.h"
#include <exception>
#include <boost/algorithm/string.hpp>
#include <filesystem/cpaf_file_directory_functions.h>

using namespace std;
namespace fs = std::filesystem;
namespace cfs = cpaf::filesystem;

namespace cpaf::semantic
{

void semantic_data_resources::clear()
{
    original_resources_.clear();
    excluded_matches_.clear();
    all_local_files_.clear();
}

void semantic_data_resources::add_excluded_file_name(const string& name)
{
    excluded_matches_.push_back(name);
}

void semantic_data_resources::add_resource(const string& resource_path)
{
    add_resource(resource_path, "");
}

void semantic_data_resources::add_resource(const std::filesystem::path& file_path)
{
    add_resource(file_path.string());
}

void semantic_data_resources::add_resource(const string& resource_path, const string& pattern)
{
    if (cfs::detect_media_location(resource_path) == cfs::media_location::local) {
        add_local_resource(resource_path, pattern);
    }
    else {
        throw std::domain_error("semantic_data_resources::add_resource: TODO: Unsupported resource media_location");
    }
}

void semantic_data_resources::add_resource(const std::filesystem::path& file_path, const string& pattern)
{
    add_resource(file_path.string(), pattern);
}

void semantic_data_resources::init()
{

}

void semantic_data_resources::add_local_resource(const string& resource_path, const string& pattern)
{
    const fs::path path(resource_path);
    if (fs::is_directory(path)) {
        const auto files = cpaf::filesystem::files_list_recursive(path, true, pattern, -1);
        for (const auto& p : files) {
            const auto leaf_name = p.filename().string();
            if (file_is_excluded(p)) {
                continue;
            }
            add_local_file(p);
        }
    }
    else {
        add_local_file(fs::path(path));
    }
}

void semantic_data_resources::add_local_file(const std::filesystem::path& file_path)
{
    all_local_files_.push_back(file_path);
}

bool semantic_data_resources::file_is_excluded(const std::filesystem::path& file_path) const
{
    using namespace boost::algorithm;
    const auto leaf_name = file_path.filename().string();
    if (starts_with(leaf_name, "_")) {
        return true; // We consider file names beginning with '_' as "debug"/non used files.
    }

    for (const auto& exclude_match_name : excluded_matches_) {
        if (!exclude_match_name.empty() && contains(leaf_name, exclude_match_name)) {
            return true;
        }
    }
    return false;
}


} //end namespace cpaf::semantic
