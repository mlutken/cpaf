#include "subtitle_container.h"
#include <Zippy.hpp>



namespace cpaf::video {

void subtitle_container::resource_path_set(const std::string& resource_path)
{
    resource_path_ = resource_path;
}

bool subtitle_container::is_zip_archive() const
{
    return resource_path_.ends_with(".zip");
//    return resource_path_.ends_with(".zip") || resource_path_.contains(".zip/");
}



} // namespace cpaf::video
