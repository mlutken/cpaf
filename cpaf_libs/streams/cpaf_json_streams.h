#ifndef CPAF_JSON_STREAMS_H
#define CPAF_JSON_STREAMS_H

#include <filesystem>
#include <utils/cpaf_json_utils.h>

namespace cpaf::streams {

/** \unit cpaf_json_streams.h
! Functions reading/writing json files.
\author Martin Lutken
*/

void            to_file (const std::string& resource_path, const nlohmann::json& jobject, int indentation = 0);
void            to_file (const std::filesystem::path& file_path, const nlohmann::json& jobject, int indentation = 0);

nlohmann::json  to_json (const std::string& resource_path, const nlohmann::json& default_jo = {});
nlohmann::json  to_json (const std::filesystem::path& file_path, const nlohmann::json& default_jo = {});

} //END namespace cpaf::streams


#endif //CPAF_JSON_STREAMS_H


