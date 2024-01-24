#include "subtitle_container.h"
#include <cpaf_libs/unicode/cpaf_u8string_utils.h>
#include <Zippy.hpp>


using namespace std;
using namespace cpaf::unicode;
using namespace std::chrono;

namespace cpaf::video {

void subtitle_container::resource_path_set(const std::string& resource_path)
{
    resource_path_ = resource_path;
}

std::string subtitle_container::archive_path() const
{
    if (is_zip_archive() && resource_path_.contains(".zip:/")) {
        return cpaf::unicode::substring_between(resource_path_, "", ".zip:/") + ".zip";
    }
    return resource_path_;
}

bool subtitle_container::is_zip_archive() const
{
//    return resource_path_.ends_with(".zip");
    return resource_path_.ends_with(".zip") || resource_path_.contains(".zip:/");
}



} // namespace cpaf::video
